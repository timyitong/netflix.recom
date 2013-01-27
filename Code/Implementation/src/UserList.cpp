#include "UserList.h"
UserList::UserList(const int &uid):
	id(uid),
	_avg(-1),
	_var(-1),
	_total_rating(0),
	_total_count(0),
	_normalized(false),
	ignored(false)
{}

void UserList::add(const int &movie_id, const int &rating){
	ScorePair ms;
	ms.id=movie_id;
	ms.score=rating;
	_list.push_back(ms);
	_total_rating+=rating;
	_total_count+=1;
}
void UserList::add(const int &movie_id, const int &rating, const float &norm){
	ScorePair ms;
	ms.id=movie_id;
	ms.score=rating;
	ms.score_norm=norm;
	_list.push_back(ms);
	_total_rating+=rating;
	_total_count+=1;
}
float UserList::var(){   // get the variance of this list
	if (_var==-1){
		if (_avg==-1)
			avg();
		vector<ScorePair>::iterator it=_list.begin();
		_var=0;
		while(it!=_list.end()){
			_var+=(it->score-_avg)*(it->score-_avg);
			it++;
		}
		_var=sqrt(_var);
		if (_var==0)
			ignored=true;
	}
	return _var;
}
float UserList::avg(){//get the average of this list
	if (_avg==-1){
		if (_total_count!=0)
			_avg=_total_rating/_total_count;
		else
			_avg=0; // actually this won's occur in our case, userlist at least have one record
	}
	return _avg;
}
vector<ScorePair>* UserList::getList(){
	return &_list;
}
int UserList::size(){  //get size
	return _list.size();
}
void UserList::normalize(){   // Pearson Correlation normalization
	if (!_normalized){
		float avg=this->avg();
		float var=this->var();
		if (var==0)
			ignored=true;
		else{
			vector<ScorePair>::iterator it=_list.begin();
			while(it!=_list.end()){
				it->score_norm=(it->score-avg)/var;
				it++;
			}
		}
		_normalized=true;
	}
}
void UserList::displayStats(ostream &out){  // display the statistical information of a userlist
	int rating_count[5]={0,0,0,0,0};
	int total_num=0;
	vector<ScorePair>::iterator it;
	for (it=_list.begin();it<_list.end();it++){
		rating_count[int((*it).score)-1]++;
		total_num++;
	}
	float avg=0;
	for (int i=0;i<5;i++)
		avg+=(i+1)*rating_count[i];
	avg=avg/total_num;
	cout<<"# of movies:"<<total_num<<endl;
	cout<<"Rates as 1:"<<rating_count[0]<<endl;
	cout<<"Rates as 3:"<<rating_count[2]<<endl;
	cout<<"Rates as 5:"<<rating_count[4]<<endl;
	cout<<"Average rating:"<<avg<<endl;
}
void UserList::displayList(){
	vector<ScorePair>::iterator it;
	for (it=_list.begin();it!=_list.end();++it)
		cout<<it->id<<","<<it->score<<","<<it->score_norm<<endl;
}