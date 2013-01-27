#ifndef SIMCACHE_H
#define SIMCACHE_H

using namespace std;
#include <map>
#include <vector>
#include <set>
#include "ScorePair.h"

class SimCache{
public:
	static SimCache* getInstance();
	void setCache(const int&);// identify current cache owner
	void addCache(const int&, const int&, const float&);
	void addSkip(const int&, const int&);

	bool checkIfOwnCache(const int &owner);
	bool checkIfOwnSkip(const int &onwer);
	bool hasOneCache(const int &user_id);

	vector<ScorePair>::iterator getCache_begin(const int&);
	vector<ScorePair>::iterator getCache_end(const int&);
	vector<ScorePair>* getCacheList(const int &owner);
	set<int>* getSkipSet(const int &owner);
	void cleanCache(const int&);
private:
	map<int,vector<ScorePair>*> _map;
	map<int, set<int>* > _skip_map;
	set<int>	_cleaned_set;
	set<int>	_get_set;
	int _owner;
	vector<ScorePair>* _p;
	//singelton:
	static void CleanUp();
	SimCache();
	~SimCache(){}
	//not copyable:
	SimCache(SimCache const&);
	SimCache& operator=(SimCache const&);

	static SimCache* MInstance;
};

#endif