/*
	Basic User-User Algorithm Class
	 -- All other Algorithm class: MovieMovieSim MovieMovieSimNorm UserUSerSimNorm MySim 
	 		all inherits this class
*/

#ifndef SIMCALCULATOR_H
#define SIMCALCULATOR_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include "MovieDictionary.h"
#include "UserDictionary.h"
#include "MovieList.h"
#include "UserList.h"
#include "ScorePair.h"
#include "SimCache.h"

class SimCalculator{
public:
	SimCalculator(string &query_url, const int k);
	void calculate(ostream &out);
protected:
	//attributes:
	string _query_url;
	map <int,vector<ScorePair>* > _map; // first level cache
	int _k;
	bool _map_open;  // idicate whether the map is open

	MovieDictionary* _movieDict; //The pointer to movie dictionary
	UserDictionary*	_userDict;   // The pointer to user dictionary
	SimCache*	 	_cache;	    // The pointer to the second level cache

	map <int,list<int>* > _map_user_movies; // simple map to remember what is appeared in query file
	map <int,list<int>* > _map_movie_users; // simple map to remember what is apearred in query file

	//method:
	static bool compareID(ScorePair s1, ScorePair s2);
	static bool compareScore(ScorePair s1, ScorePair s2);
	virtual float getScore(const int &user_id, const int &movie_id);

	virtual int getPK(const int &user_id, const int &movie_id);
	virtual float handleScore(float score, float var1, float var2);

	virtual vector<ScorePair>* getSimList(const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getRatingList(const int &user_id, const int &movie_id);
	virtual float getFirstAvg(const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getFirstList(const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getFirstList(const int &be_id, const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getSecondList(const int &user_id, const int &movie_id);

	virtual vector<ScorePair>* getTopK(const int &user_id, const int &movie_id);
	virtual void getCalculate_set(const int &user_id, const int &movie_id,set<int> &cal_set, vector<ScorePair>* &result);


};

#endif