/*
ATTENDTION::::!!!!!!!!!!!!!!!!!!!!!!!!!!!!
This Class is currently not in use and not included in makefile.
Because the useruser normalzation algorithm is performing really bad that I finalyy chooce to use moviemovie normalization algorithm



*/

#include "UserUserSimNorm.h"
float UserUserSimNorm::getScore(const int &user_id, const int &movie_id){
	vector<ScorePair>* sim_list=this->getSimList(user_id,movie_id);
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
	if (weight==0) 
		score=0;
	else
		score=score/weight;
	
	UserList* owner=_userDict->getUser(user_id);
	//MovieList* owner=_movieDict->getMovie(movie_id);
	
	//score=score*owner->var()+owner->avg();
	score=score+owner->avg();
	return score;
}

vector<ScorePair>* UserUserSimNorm::getRatingList(const int &user_id, const int &movie_id){
	MovieList* l=_movieDict->getMovie(movie_id);
	l->sortList();
	vector<ScorePair>* result=new vector<ScorePair>;
	vector<ScorePair>::iterator it;
	float var;
	float avg;
	for (it=l->begin();it!=l->end();++it){
		UserList * ul=_userDict->getUser(it->id);
		var=ul->var();
		avg=ul->avg();
		ScorePair sp;
		sp.id=it->id;
	   // this is the Normalization for Step 2: rating calculation
		/*if (var==0)  // only 1 situation will happen this: The list has scores all the same!
			sp.score=0;
		else
			sp.score=(it->score-avg)/var;
		*/sp.score=it->score-avg;

		result->push_back(sp);
	}
	return result;
}
float UserUserSimNorm::handleScore(float score, float var1, float var2){
	/* cosine:*/
	if (var1*var2==0)  // when var1*var2=0 
		return 0;
	score=score/sqrt(var1*var2);
	return fabs(score);
}
