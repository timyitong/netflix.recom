#ifndef USERLIST_H
#define USERLIST_H
using namespace std;
#include <vector>
#include <iostream>
#include <math.h>
#include "ScorePair.h"

class UserList{
public:
	const int id; // the movie id
	bool ignored; // whether this user list should be ignored
	UserList(const int &uid);
	void add(const int &movie_id, const int &rating);
	void add(const int &movie_id, const int &rating, const float &norm);
	vector<ScorePair>* getList();
	void displayStats(ostream &out);

	vector<ScorePair>::iterator begin(){ return _list.begin();}
	vector<ScorePair>::iterator end(){return _list.end();}
	float var();
	float avg();
	void normalize();

	void displayList();
	int size();

private:
	vector <ScorePair> _list;
	float _avg;
	float _var;
	int _total_rating;
	int _total_count;
	bool _normalized;  //tag whether current list already be normalized
};

#endif