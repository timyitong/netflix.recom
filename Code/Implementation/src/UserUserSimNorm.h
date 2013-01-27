#ifndef USERUSERSIMNORM_H
#define USERUSERSIMNORM_H

#include "SimCalculator.h"

class UserUserSimNorm : public SimCalculator{
public:
	//constructor:
	UserUserSimNorm(string &s, const int &k): SimCalculator(s,k){ }
protected:
	virtual float getScore(const int &user_id, const int &movie_id);
	virtual float handleScore(float score, float var1, float var2);
	virtual vector<ScorePair>* getRatingList(const int &user_id, const int &movie_id);
private:

};

#endif