#include "UserDictionary.h"
#include <stdlib.h>
//Singelton Implementation:
UserDictionary* UserDictionary::MInstance = 0;
UserDictionary::UserDictionary(){atexit(&CleanUp);}
void UserDictionary::CleanUp(){delete MInstance; MInstance=0;}
UserDictionary* UserDictionary::getInstance(){
	if (MInstance==0)
		MInstance = new UserDictionary();
	return MInstance;
}

//add and get operation
bool UserDictionary::existUser(const int &id){
	if ( _id_set.find(id) == _id_set.end() )
		return false;
	else 
		return true;
}
void UserDictionary::addUser(const int &id, UserList *&p){
	_id_set.insert(id);
	_id_map.insert(pair<int, UserList*>(id,p));
}
UserList* UserDictionary::getUser(const int &id){
	return _id_map[id];
}
UserList* UserDictionary::getUserNormalized(const int &id){
	UserList* ul=_id_map[id];
	ul->normalize();
	return ul;
}
