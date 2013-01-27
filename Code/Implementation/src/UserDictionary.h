#ifndef USERDICTIONARY_H
#define USERDICTIONARY_H 

using namespace std;
#include "UserList.h"
#include <vector>
#include <map>
#include <set>
#include <math.h>

/*Here orginally using hashmap is faster, in which case we shall use std::tr1::map, but this is not guranteed
  in all environment. So I change it back into tree map:*/
/*
#include <tr1/unordered_map>
typedef tr1::unordered_map <int,UserList *> hashmap;
*/
typedef map <int,UserList *> hashmap;  

class UserDictionary{  // a singelton
public:
	static UserDictionary* getInstance();

	void addUser(const int &id,UserList *&p);
	bool existUser(const int &);
	UserList* getUser(const int &id); 
	UserList* getUserNormalized(const int &id);

 	hashmap::iterator begin(){ return _id_map.begin();}
	hashmap::iterator end(){return _id_map.end();}
private:
	hashmap	_id_map;
	set <int>	_id_set;
	static void CleanUp();
	UserDictionary();
	~UserDictionary(){}

	//not copyable:
	UserDictionary(UserDictionary const&);
	UserDictionary& operator=(UserDictionary const&);

	static UserDictionary* MInstance;

};


#endif