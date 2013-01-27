#include "MovieList.h"
MovieList::MovieList(const int &mid):
	id(mid),
	_unsorted(true),
	_normalized(false),
	_var(-1),
	_avg(-1),
	_total_count(0),
	_total_num(0)
{}

void MovieList::add(const int &id, const int &score){
	ScorePair us;
	us.id=id;
	us.score=score;
	_list.push_back(us);
	_total_count+=score;
	_total_num+=1;
}
int MovieList::size(){
	return _list.size();
}
void MovieList::displayStats(ostream &out){  //display the statistical informaiton for a certain movie
	int rating_count[5]={0,0,0,0,0};
	int total_num=0;
	vector<ScorePair>::iterator it;
	for (it=_list.begin();it!=_list.end();it++){
		rating_count[int((*it).score)-1]++;
		total_num++;
	}
	float avg=0;
	for (int i=0;i<5;i++)
		avg+=(i+1)*rating_count[i];
	avg=avg/total_num;
	cout<<"# of users:"<<total_num<<endl;
	cout<<"Rates as 1:"<<rating_count[0]<<endl;
	cout<<"Rates as 3:"<<rating_count[2]<<endl;
	cout<<"Rates as 5:"<<rating_count[4]<<endl;
	cout<<"Average rating:"<<avg<<endl;
}
vector<ScorePair>* MovieList::getList(){
	sortList();
	return &_list;
}
struct compare{
	bool operator()(ScorePair u1,ScorePair u2){
		return (u1.id<u2.id);
	}
}compareUserID;
void MovieList::sortList(){ // sort if unsorted before
	if (_unsorted){
		sort(_list.begin(),_list.end(),compareUserID);
		_unsorted=false;
	}
}
void MovieList::normalize(){
	if (!_normalized){
		float avg=this->avg();
		float var=this->var();
		vector<ScorePair>::iterator it;
		for (it=_list.begin();it!=_list.end();++it){
			if (var!=0)
				it->score_norm=(it->score-avg)/var;
			else
				it->score_norm=0;
		}
		_normalized=true;
	}
}
float MovieList::avg(){   // return the average rating
	if (_avg==-1)
		_avg=_total_count/_total_num;
	return _avg;
} 
float MovieList::var(){ // return the variance of rating
	if (_var==-1){
		float avg=this->avg();
		vector<ScorePair>::iterator it;
		float var_sqr=0;
		for (it=_list.begin();it!=_list.end();++it)
			var_sqr+=(it->score-avg)*(it->score-avg);
		_var=sqrt(var_sqr);
	}
	return _var;
}