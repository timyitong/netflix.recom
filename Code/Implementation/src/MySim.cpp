/*
This is the Self-made algorithm Class



Inherits from SimCalculator, but I mainly rewrites most of the functions:

*/
#include "MySim.h"

bool combine(ScorePair p1, ScorePair p2){
	return p1.score_norm>p2.score_norm;
}
float MySim::getScore(const int &user_id, const int &movie_id){   // calculate out the score
	vector<ScorePair> total_sim;
	vector<ScorePair>* u=getTopKUsers(user_id); // we need user similarity list for calculation
	vector<ScorePair>* m=getTopKMovies(movie_id); // we need movie similarity list for calculation

	vector<ScorePair>::iterator u_it;
	vector<ScorePair>::iterator it2;
	vector<ScorePair>::iterator m_it;
	for (u_it=u->begin();u_it!=u->end();u_it++){
		int uid=u_it->id;
		UserList* ul=_userDict->getUser(u_it->id);
		m_it=m->begin();
		it2=ul->begin();  // This is the only one has the true rating scores!
		float s1;
		float s2;
		while(m_it!=m->end() && it2!=ul->end()){
			if (m_it->id > it2->id){
				++it2;
			}else if (m_it->id < it2->id){
				++m_it;
			}else{// they are equal
				int mid=m_it->id;
				ScorePair sp;
				MovieList* ml=_movieDict->getMovie(mid);
				float var1=ul->var();
				float var2=ml->var();
				if (var1*var2!=0)
					sp.score=(it2->score-(ul->avg()+ml->avg())/2)/sqrt(var1*var2); // This is the true rating scores
				else
					sp.score=0;
				s1=u_it->score_norm;//These two are similarity scores
				s2=m_it->score_norm;
				sp.score_norm=s1*s2;
				total_sim.push_back(sp);
				++m_it;
				++it2;
			}
		}
	}
	int k=_k/2;
	if (k>total_sim.size()) k=total_sim.size();
	partial_sort(total_sim.begin(),total_sim.begin()+k,total_sim.end(),combine);
	total_sim.resize(k);
	vector<ScorePair>::iterator it;
	float score=0;
	float weight=0;
	for (it=total_sim.begin();it!=total_sim.end();++it){
		score+=it->score*it->score_norm;
		weight+=it->score_norm;
	}
	if (weight!=0)
		score=score/weight;

	float avg_u=_userDict->getUser(user_id)->avg();
	float avg_m=_movieDict->getMovie(movie_id)->avg();
	float var_u=_userDict->getUser(user_id)->var();
	float var_m=_movieDict->getMovie(movie_id)->var();

	score=score*sqrt(var_u*var_m)+(avg_m+avg_u)/2;

	return swiftScore(user_id, movie_id, score);
}
float MySim::swiftScore(const int &user_id, const int &movie_id, float &score){ 
// This is a little tricky to improve 0.00001, round score to integers if they are very close to the nearest integers
	int low=(int)score;
	if (score-low<0.1)
		score=low;
	if (low+1-score<0.1)
		score=low+1;
	return score;
}


//get the Top K most similar USers
vector<ScorePair>* MySim::getTopKUsers(const int &user_id){
	if (_map_user.find(user_id)==_map_user.end()){
	map<int,UserList *>::iterator it;
	UserList* ul=_userDict->getUserNormalized(user_id);

	vector<ScorePair>* result=new vector<ScorePair>;
	
	vector<ScorePair>::iterator it1;
	vector<ScorePair>::iterator it2;
	for (it=_userDict->begin();it!=_userDict->end();++it){ // calculate the similarity metric
		UserList* be_ul=it->second;
		be_ul->normalize();
		int be_id=it->first;
		it1=ul->begin();
		it2=be_ul->begin();
		float score=0;
		float s1;
		float s2;
		while(it1!=ul->end() && it2!=be_ul->end()){
			if (it1->id>it2->id){
				++it2;
			}else if (it1->id<it2->id){
				++it1;
			}else{// they are equal
				s1=it1->score_norm;
				s2=it2->score_norm;
				score+=s1*s2;
				++it2;
				++it1;
			}
		}
		if (score!=0){
			ScorePair sp;
			sp.id=be_id;
			sp.score=fabs(score);
			sp.score_norm=score;
			result->push_back(sp);
		}
	}
	int k=_k;
	if (k>result->size())
		k=result->size();
	partial_sort(result->begin(),result->begin()+k,result->end(),SimCalculator::compareScore);
	//store these users into the user_sim vector
	result->resize(k);
	sort(result->begin(),result->end(),SimCalculator::compareID);
	_map_user.insert(make_pair(user_id,result));
	return result;
	}else{
		return _map_user[user_id];
	}
}

// get the top K most similiar movies
vector<ScorePair>* MySim::getTopKMovies(const int &movie_id){
	if (_map_movie.find(movie_id)==_map_movie.end()){
	map<int,MovieList *>::iterator it;
	MovieList* ul=_movieDict->getMovieNormalized(movie_id);
	ul->sortList();

	vector<ScorePair>* result=new vector<ScorePair>;
	
	vector<ScorePair>::iterator it1;
	vector<ScorePair>::iterator it2;
	for (it=_movieDict->begin();it!=_movieDict->end();++it){
		MovieList* be_ul=it->second;
		be_ul->sortList();
		be_ul->normalize();
		int be_id=it->first;
		it1=ul->begin();
		it2=be_ul->begin();
		float score=0;
		/*Pearson:*/
		float score1=0;
		float s1;
		float s2;
		/*Cosine:*/
		float score2=0;
		float var1=0;
		float var2=0;
		float ss1=0;
		float ss2=0;
		while(it1!=ul->end() && it2!=be_ul->end()){// calculate the similarity metric
			if (it1->id > it2->id){
				//cosine:
				ss2=it2->score;
				var2+=ss2*ss2;
				++it2;
			}else if ( it1->id < it2->id){
				//cosine:
				ss1=it1->score;
				var1+=ss1*ss1;

				++it1;
			}else{// they are equal
				//pearson:
				s1=it1->score_norm;
				s2=it2->score_norm;
				score1+=s1*s2;
				//cosine:
				ss1=it1->score;
				ss2=it2->score;
				score2+=ss1*ss2;
				var1+=ss1*ss1;
				var2+=ss2*ss2;

				++it1;
				++it2;
			}
		}
		if (var1*var2!=0)
			score2=score2/sqrt(var1*var2);
		else
			score2=0;
		score=score1+0.8*score2;
		if (score!=0){
			ScorePair sp;
			sp.id=be_id;
			sp.score=fabs(score);
			sp.score_norm=score;   // the score is for sort, the score_num is the true similarity score
			result->push_back(sp);
		}
	}
	int k=_k;
	if (k>result->size())
		k=result->size();
	partial_sort(result->begin(),result->begin()+k,result->end(),SimCalculator::compareScore);
	//store these users into the user_sim vector
	result->resize(k);
	sort(result->begin(),result->end(),SimCalculator::compareID);
	_map_movie.insert(make_pair(movie_id,result));
	return result;
	}else{
		return _map_movie[movie_id];
	}
}