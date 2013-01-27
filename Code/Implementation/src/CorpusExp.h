/* CorpusExporater header file
   Used for statistical purpose

*/
#ifndef CORPUSEXP_H
#define CORPUSEXP_H

using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>  // the Linux library to read the directories
#include <string.h>
#include <stdio.h>
#include "MovieList.h"
#include "MovieDictionary.h"
#include "UserDictionary.h"

class CorpusExp{
public:
	CorpusExp(const string&,const string&); // (dir_name, mode)
	void calculate();
	void displayUser(ostream &out, const int&);    // print out the id's stat information
	void displayMovie(ostream &out, const int&);
	void displayAll(ostream &out);
private:
	string _dir_name;
	int _movie_count;
	int _user_count;
	int _pair_count;
	int _movie_rated_count[5]; // times of movie rated as 1,2,3,4,5
	float _movie_avg;   // total moview average rates
	string _mode; // the mode of current task
	int _id; // the id of current task

	inline void read_one_file(const string&);

};

#endif