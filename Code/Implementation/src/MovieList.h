#ifndef MOVIELIST_H
#define MOVIELIST_H
using namespace std;
#include <vector>
#include <iostream>
#include <algorithm>
#include "ScorePair.h"
#include "math.h"

class MovieList{
public:
	const int id; // the movie id
	MovieList(const int &mid);
	void add(const int &user_id, const int &rating);
	void displayStats(ostream &out);
	void sortList();

	vector<ScorePair>::iterator begin(){ return _list.begin();}
	vector<ScorePair>::iterator end(){return _list.end();}
	vector<ScorePair>* getList();
	float var();
	float avg();
	void normalize();
	int size();
private:
	vector <ScorePair> _list;
	bool _normalized;
	bool _unsorted;
	float _avg;
	float _var;
	float _total_count;
	int   _total_num;
};

#endif