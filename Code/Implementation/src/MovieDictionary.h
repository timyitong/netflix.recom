#ifndef MOVIEDICTIONARY_H
#define MOVIEDICTIONARY_H

using namespace std;
#include "MovieList.h"
#include <vector>
#include <map>
#include <math.h>
/*Here orginally using hashmap is faster, in which case we shall use std::tr1::map, but this is not guranteed
  in all environment. So I change it back into tree map:*/
/*
#include <tr1/unordered_map>
typedef tr1::unordered_map<int, MovieList* > movie_hashmap;
*/
typedef map<int, MovieList* > movie_hashmap;

class MovieDictionary{  // a singelton
public:
	static MovieDictionary* getInstance();

	void addMovie(const int &id,MovieList *&p);
	MovieList* getMovie(const int &id); 
	MovieList* getMovieNormalized(const int &id);

	movie_hashmap::iterator begin(){ return _id_map.begin();}
	movie_hashmap::iterator end(){return _id_map.end();}
private:
	movie_hashmap _id_map;
	static void CleanUp();
	MovieDictionary();
	~MovieDictionary(){}

	//not copyable:
	MovieDictionary(MovieDictionary const&);
	MovieDictionary& operator=(MovieDictionary const&);

	static MovieDictionary* MInstance;

};

#endif