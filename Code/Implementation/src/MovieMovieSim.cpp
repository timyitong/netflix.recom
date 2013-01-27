#include "MovieMovieSim.h"

/*This is the Implementation Class for the Movie-Movie Simmilarity recommendation
	Since inherit from the SimCalculator class, we only need to override same key functions:
*/

int MovieMovieSim::getPK(const int &user_id,const int &movie_id){ 
	return movie_id;
}
vector<ScorePair>* MovieMovieSim::getRatingList(const int &user_id, const int &movie_id){
	return _userDict->getUser(user_id)->getList();
}
float MovieMovieSim::getFirstAvg(const int &user_id, const int &movie_id){
	return _movieDict->getMovie(movie_id)->avg();
}
/*
view Movie List as First List
view User List as seond List
*/
vector<ScorePair>* MovieMovieSim::getFirstList(const int &user_id, const int &movie_id){ 
	MovieList* l=_movieDict->getMovieNormalized(movie_id);
	l->sortList();
	return l->getList();
}
vector<ScorePair>* MovieMovieSim::getFirstList(const int &be_id, const int &user_id, const int &movie_id){
	MovieList* l=_movieDict->getMovieNormalized(be_id);
	l->sortList();
	return l->getList();
}
vector<ScorePair>* MovieMovieSim::getSecondList(const int &user_id,const int &movie_id){
	return _userDict->getUser(user_id)->getList();
}
void MovieMovieSim::getCalculate_set(const int &user_id, const int &movie_id,set<int> &cal_set, vector<ScorePair>* &result){
	int oid=getPK(user_id,movie_id);
	if (_cache->checkIfOwnCache(oid))
		result=_cache->getCacheList(oid);
	else
		result=new vector<ScorePair>;
	
	map<int, MovieList*>::iterator it;
	for (it=_movieDict->begin();it!=_movieDict->end();it++){
		int id=it->first;
		if ( !_cache->hasOneCache(id) && cal_set.find(id)==cal_set.end()){ //do not need to skip and do not have cache has not yet been added
			cal_set.insert(id);
		}
	}
	
	list<int>* user_set=_map_movie_users[oid];
	list<int>::iterator user_it;
	for (user_it=user_set->begin();user_it!=user_set->end();++user_it){
		UserList* ml=_userDict->getUser((*user_it));
		vector<ScorePair>::iterator it2;
		for (it2=ml->begin();it2!=ml->end();++it2){
			int id=it2->id;
			if ( !_cache->hasOneCache(id) && cal_set.find(id)==cal_set.end()){ //do not need to skip and do not have cache has not yet been added
				cal_set.insert(id);
			}
		}
	}
}
