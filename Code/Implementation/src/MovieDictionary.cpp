#include "MovieDictionary.h"
//Singelton Implementation:
MovieDictionary* MovieDictionary::MInstance = 0;
MovieDictionary::MovieDictionary(){atexit(&CleanUp);}
void MovieDictionary::CleanUp(){delete MInstance; MInstance=0;}
MovieDictionary* MovieDictionary::getInstance(){
	if (MInstance==0)
		MInstance = new MovieDictionary();
	return MInstance;
}

//add and get operation
void MovieDictionary::addMovie(const int &id, MovieList *&p){ 
	_id_map.insert(pair<int, MovieList*>(id,p));
}
MovieList* MovieDictionary::getMovie(const int &id){// get the MovieList
	movie_hashmap::iterator it=_id_map.find(id);
	if (it==_id_map.end())
		return 0;
	return (it->second);
}
MovieList* MovieDictionary::getMovieNormalized(const int &id){ // The difference between this method and the getMovie is that it will normalize the score
	movie_hashmap::iterator it=_id_map.find(id);
	if (it==_id_map.end())
		return 0;
	else{
		it->second->normalize();
		return (it->second);
	}
}
