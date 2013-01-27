/*
	Basic User-User Algorithm Class
	 -- All other Algorithm class: MovieMovieSim MovieMovieSimNorm UserUSerSimNorm MySim 
	 		all inherits this class
*/

#include "SimCalculator.h"
	//constructor:
SimCalculator::SimCalculator(string &query_url, const int k):
	_query_url(query_url),
	_map_open(true),
	_k(k)
{	//Get the Singelton instances for the Maps of userlists movielist and cache
	_movieDict=MovieDictionary::getInstance();
	_userDict=UserDictionary::getInstance();
	_cache=SimCache::getInstance();


	//Prepare two simple map for future cache save:
	// we store two things: (1)for each user in the query file, we collect all the movie_id it has appeared in the query file
	//                      (2) for each movie in the query file, we collect all the user_id it has appeared in the query file
	ifstream infile(_query_url.c_str());   // readin file once and build a user_id(movie_id list)
	string line;
	int movie_id;
	int user_id;

	list<int>* m;
	while(infile>>line){
		if (line[line.length()-1]==':'){
			movie_id=atoi(line.c_str());
			m=new list<int>;
			_map_movie_users.insert(make_pair(movie_id,m));
		}
		else{
			user_id=atoi(line.c_str());
			list<int>* p;
			if (_map_user_movies.find(user_id)==_map_user_movies.end()){
				p=new list<int>;
				_map_user_movies.insert(make_pair(user_id,p));
			}else
				p=_map_user_movies[user_id];
			p->push_back(movie_id);	//add current movie_id to the user->movies mapping
			m->push_back(user_id);	//add current user_id to the movie->users mapping
		}
	}
}
void SimCalculator::calculate(ostream &out){ // The entrance of this Class, from here we began calculation
	ifstream infile(_query_url.c_str());
	string line;
	int movie_id;
	int user_id;
	MovieList* ml;
	while(infile>>line){
		if (line[line.length()-1]==':'){
			movie_id=atoi(line.c_str());
			out<<line<<'\n';//write down the movie_id
		}
		else{
			user_id=atoi(line.c_str());
			float score=getScore(user_id,movie_id);  //calculate the score
			out<<score<<'\n';//write down the score
		}
	}
}
float SimCalculator::getScore(const int &user_id, const int &movie_id){ //This is the function for calculating the score (the step [2] of the algorithm)
	vector<ScorePair>* sim_list=this->getSimList(user_id,movie_id);     // This is the function to get the Top K items
	vector<ScorePair>* rating_list=this->getRatingList(user_id,movie_id);
	vector<ScorePair>::iterator sim_it=sim_list->begin();
	vector<ScorePair>::iterator rating_it=rating_list->begin();
	int sid;
	int rid;
	float weight=0;
	float score=0;
	while (sim_it!=sim_list->end() && rating_it!=rating_list->end()){
		sid=sim_it->id;
		rid=rating_it->id;
		if (sid==rid){
			score+=sim_it->score*rating_it->score;
			weight+=sim_it->score;
			sim_it++;
			rating_it++;
		}else if (sid<rid){
			sim_it++;
		}else{
			rating_it++;
		}
	}
	if (weight!=0)
		score=score/weight;
	else
		score=getFirstAvg(user_id,movie_id);
	
	return score;
}
vector<ScorePair>* SimCalculator::getSimList(const int &user_id, const int &movie_id){// get the Top K Iterms
	int id=getPK(user_id,movie_id);	// Check if in the map, if not, we need new calculation; if yes, we directly get it from the cache
	if (_map_open && _map.find(id)!=_map.end())
		return _map[id];
	else 	 //if cannot find cached top k list
		return getTopK(user_id,movie_id);	
}
vector<ScorePair>* SimCalculator::getRatingList(const int &user_id, const int &movie_id){
	return _movieDict->getMovie(movie_id)->getList();
}

//This 4 functions is desinged for support both User-User and Movie-Movie inheritance:
// If we are doing the User-User, we call the User List 1st List, the Movie List the 2nd List;
// If we are doing the Movie-Movie, we call the Movie List 1st List, the User List the 2nd List.
float SimCalculator::getFirstAvg(const int &user_id, const int &movie_id){
	return _userDict->getUser(user_id)->avg();
}
vector<ScorePair>* SimCalculator::getFirstList(const int &user_id, const int &movie_id){
	return _userDict->getUserNormalized(user_id)->getList();   //notice we user normalized list, which mean the similarity metirc is pearson correlation
}
vector<ScorePair>* SimCalculator::getFirstList(const int &be_id, const int &user_id, const int &movie_id){
	return _userDict->getUserNormalized(be_id)->getList();
}
vector<ScorePair>* SimCalculator::getSecondList(const int &user_id,const int &movie_id){
	return _movieDict->getMovie(movie_id)->getList();
}
/*-----------------------*/

//This two function is for sort function in the getTopk()
bool SimCalculator::compareScore(ScorePair s1, ScorePair s2){
	return fabs(s1.score)>fabs(s2.score);
}
bool SimCalculator::compareID(ScorePair s1, ScorePair s2){
	return s1.id<s2.id;
}
vector<ScorePair>* SimCalculator::getTopK(const int &user_id, const int &movie_id){ // get the top K items for similarity calculation

	int PK=getPK(user_id,movie_id);
	vector<ScorePair>* result;

	vector<ScorePair>* ul=this->getFirstList(user_id,movie_id);
	set<int> cal_set;
	getCalculate_set(user_id,movie_id,cal_set,result); // before calculation, we need to get the list of ids that actually needs to be calculated
	set<int>::iterator it;
	vector<ScorePair>::iterator it1;
	vector<ScorePair>::iterator it2;
	int index=0;
	for (it=cal_set.begin();it!=cal_set.end();++it){
		int be_id=(*it);
		ScorePair sp;
		it1=ul->begin();
		vector<ScorePair>* be_ul=this->getFirstList(be_id,user_id,movie_id);
		it2=be_ul->begin();

		float score=0;
		float s1=0;
		float s2=0;
		float var1=0;
		float var2=0;
		while ( it1!=ul->end() && it2!=be_ul->end() ){  // calculate the similartity
			s1=it1->score_norm;
			s2=it2->score_norm;
			if (it1->id == it2->id){
				score+=s1*s2;
				var1+=s1*s1;
				var2+=s2*s2;
				it1++;
				it2++;
			}else if ( it1->id > it2->id){
				var2+=s2*s2;
				it2++;
			}else{
				var1+=s1*s1;
				it1++;
			}
		}
		score=handleScore(score,var1,var2);  // This function is designed to both support Pearson Correlation and cosine similarity
		if (score!=0){
			sp.id=be_id;
			sp.score=score;
			result->push_back(sp);
		}
		_cache->addCache(be_id,PK,score);
	}
	int k=_k;
	if (k>result->size())
		k=result->size();
	partial_sort(result->begin(),result->begin()+k,result->end(),SimCalculator::compareScore);
	result->resize(k); //resize the result vector and only contains the top k values
	sort(result->begin(),result->end(),SimCalculator::compareID); //sort it by ID

	_map.insert(make_pair(PK,result));

	return result;
}
// Tell  whether user_id or movie_id is the primary key, in User-User model, the PK should be user_id; in Movie-Movie model, the PK should be movie_id
int SimCalculator::getPK(const int &user_id,const int &movie_id){ 
	return user_id;
}
void SimCalculator::getCalculate_set(const int &user_id, const int &movie_id,set<int> &cal_set, vector<ScorePair>* &result){
	int oid=getPK(user_id,movie_id);
	if (_cache->checkIfOwnCache(oid)) //check the second level cache to see if we have something already calculated
		result=_cache->getCacheList(oid);
	else
		result=new vector<ScorePair>;
	
	list<int>* movie_set=_map_user_movies[oid];
	list<int>::iterator movie_it;
	for (movie_it=movie_set->begin();movie_it!=movie_set->end();++movie_it){ // checkt the current query_word's movie's list, we only need to calculate those user_id in the movie list, otherwise, the final predicted score will be 0, nonsense to bother.
		MovieList* ml=_movieDict->getMovie((*movie_it));
		vector<ScorePair>::iterator it2;
		for (it2=ml->begin();it2!=ml->end();++it2){
			int uid=it2->id;
			if ( !_cache->hasOneCache(uid) && cal_set.find(uid)==cal_set.end()){ //do not need to skip and do not have cache has not yet been added
				cal_set.insert(uid);
			}
		}
	}
	
}
float SimCalculator::handleScore(float score, float var1, float var2){
	/* cosine:*/
	if (var1*var2==0)  // when var1*var2=0 
		return 0;
	score=score/sqrt(var1*var2);
	return fabs(score);
}