#ifndef MOVIEMOVIESIM_H
#define MOVIEMOVIESIM_H

#include "SimCalculator.h"

class MovieMovieSim : public SimCalculator{
public:
	//constructor:
	MovieMovieSim(string &s,const int &k) : SimCalculator(s,k){}
protected:
	virtual int getPK(const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getRatingList(const int &user_id, const int &movie_id);
	virtual float getFirstAvg(const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getFirstList(const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getFirstList(const int &be_id, const int &user_id, const int &movie_id);
	virtual vector<ScorePair>* getSecondList(const int &user_id, const int &movie_id);
	//virtual vector<ScorePair>* getTopK(const int &user_id, const int &movie_id);
	virtual void getCalculate_set(const int &user_id, const int &movie_id,set<int> &cal_set, vector<ScorePair>* &result);
private:


};
#endif