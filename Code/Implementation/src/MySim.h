/*
This is the Self-made algorithm Class
*/
#ifndef MYSIM_H
#define MYSIM_H

#include "SimCalculator.h"

class MySim:public SimCalculator{
public:
	MySim(string &s,const int &k): SimCalculator(s,k){}
protected:
	virtual float getScore(const int &user_id, const int &movie_id);
	vector<ScorePair>* getTopKUsers(const int &user_id);
	vector<ScorePair>* getTopKMovies(const int &movie_id);
	void getCalSetUser(const int &user_id, set<int>* cal_set);
private:
	map<int, vector<ScorePair>*> _map_user; // The cache to store the calculated top k users
	map<int, vector<ScorePair>*> _map_movie; // The cache to store the calculated top k movies
	float swiftScore(const int&, const int&, float &);

};

#endif