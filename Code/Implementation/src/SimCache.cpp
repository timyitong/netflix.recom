#include "SimCache.h"
#include <stdlib.h>
/*
This is the cache class to store the sim(a,b) systemtric score
NOTICE that this class support both movie id and user id, they are all regarded as ids and treated with no difference . 
And this class is implemented in singelton, which means you cannot use both cache at the same time.


*/
void SimCache::setCache(const int &owner){ //set current cache pointer to a certain owner
	_owner=owner;
	if ( _cleaned_set.find(owner)==_cleaned_set.end()){ // only when we need to store the cache, we store
		vector<ScorePair> *p;
		if (_map.find(owner)==_map.end()){ // if never stored the cache
			p=new vector<ScorePair>;
			_map.insert(pair<int,vector<ScorePair>*>(owner,p));
		}else
			p=_map[owner];
		_p=p;
	}else{
		_p=0;
	}
}
void SimCache::addCache(const int &owner, const int &uid, const float &score){  //add one cache to the map
	setCache(owner);
	if (_p!=0){ // only when we need to store the cache, we store
		ScorePair us;
		us.id=uid;
		us.score=score;
		_p->push_back(us);
	}
}
void SimCache::addSkip(const int &owner, const int &id){
	map<int,set<int>* >::iterator it=_skip_map.find(owner);
	set<int>* s;
	if (it==_skip_map.end()){
		s=new set<int>;
		s->insert(id);
		_skip_map.insert(make_pair(owner,s));
	}else{
		s=it->second;
		s->insert(id);
	}
}
bool SimCache::checkIfOwnSkip(const int &owner){
	if (_skip_map.find(owner)==_skip_map.end())
		return false;
	else return true;
}
set<int>* SimCache::getSkipSet(const int &owner){
	return _skip_map[owner];
}

bool SimCache::checkIfOwnCache(const int &owner){
	_get_set.clear();
	if (_map.find(owner)==_map.end()) //does not have Cache
		return false;

	vector<ScorePair> *p=_map[owner];
	vector<ScorePair>::iterator it;
	for (it=p->begin();it!=p->end();++it){
		_get_set.insert((*it).id);
	}
	return true;
}
bool SimCache::hasOneCache(const int &user_id){
	if (_get_set.find(user_id)==_get_set.end())
		return false;
	else
		return true;
}
vector<ScorePair>::iterator SimCache::getCache_begin(const int &owner){
	return _map[owner]->begin();
}
vector<ScorePair>::iterator SimCache::getCache_end(const int &owner){
	return _map[owner]->end();
}
vector<ScorePair>* SimCache::getCacheList(const int &owner){
	return _map[owner];
}
void SimCache::cleanCache(const int &owner){
	_cleaned_set.insert(owner);
	delete _map[owner];
	_map.erase(owner);
}


//Singelton Implementation:
SimCache* SimCache::MInstance = 0;
SimCache::SimCache(){atexit(&CleanUp);}
void SimCache::CleanUp(){delete MInstance; MInstance=0;}
SimCache* SimCache::getInstance(){
	if (MInstance==0)
		MInstance = new SimCache();
	return MInstance;
}
