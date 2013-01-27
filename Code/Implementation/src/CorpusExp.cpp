/*CorpusExporater cpp file
   Used for statistical purpose

*/
#include "CorpusExp.h"

CorpusExp::CorpusExp(const string &dir_name, const string &mode):
	_dir_name(dir_name),
	_mode(mode),
	_movie_count(0),
	_user_count(0),
	_pair_count(0)
{
	for (int i=0;i<5;i++)
		_movie_rated_count[i]=0;
}


void CorpusExp::calculate(){// This is the function to read the training data set
	vector<string> files;

	DIR *dpdf;
	struct dirent *epdf;
	dpdf = opendir(strdup(_dir_name.c_str()));
	if (dpdf!=NULL){
		while(epdf = readdir(dpdf)){
			string file_name=epdf->d_name;
			if (file_name.length()>=4 && file_name.substr(file_name.length()-4,file_name.length()).compare(".txt")==0)
				files.push_back(file_name);
		}
	}
	std::sort(files.begin(),files.end());
	vector<string>::iterator it;
	for (it=files.begin();it!=files.end();it++){
		read_one_file((*it));
	}
}
void CorpusExp::displayAll(ostream &out){ // the funciton to generate the general statistical data
	out<<"Total# of Movies:"<<_movie_count<<endl;
	out<<"Total# of Users:"<<_user_count<<endl;
	out<<"Rated as 1:"<<_movie_rated_count[0]<<endl;
	out<<"Rated as 2:"<<_movie_rated_count[1]<<endl;
	out<<"Rated as 3:"<<_movie_rated_count[2]<<endl;
	out<<"Rated as 4:"<<_movie_rated_count[3]<<endl;
	out<<"Rated as 5:"<<_movie_rated_count[4]<<endl;
	float avg=0;
	for (int i=0;i<5;i++)
		avg+=_movie_rated_count[i]*(i+1);
	avg=avg/_pair_count;
	out<<"Total Average rating:"<<avg<<endl;
}
void CorpusExp::displayUser(ostream &out, const int &id){
	UserDictionary::getInstance()->getUser(id)->displayStats(out);
}
void CorpusExp::displayMovie(ostream &out, const int &id){
	MovieDictionary::getInstance()->getMovie(id)->displayStats(out);
}

inline void CorpusExp::read_one_file(const string &file_name){
	_movie_count++; // STATISTICS_OP: +1 movie

	string file_url=_dir_name+"/"+file_name;
	ifstream infile(file_url.c_str());
	string line;
	infile>>line; // read the movie id
	line=line.substr(0,line.length()-1);
	int mid=atoi(line.c_str());// the movie_id
	MovieList* ml=new MovieList(mid); // new a MovieList
	MovieDictionary::getInstance()->addMovie(mid,ml);

	while (infile>>line){  // for each line of the record
		char * token;
		token=strtok (strdup(line.c_str()),",");
		int index=1;
		int user_id;
		int rating;
		while (token!=NULL && index<=2){
			if (index==1){ // get the user_id
				user_id=atoi(token);
			}else if (index==2){ //get the rating
				rating=atoi(token);
			}
			token=strtok(NULL,",");
			index++;
		}
		ml->add(user_id,rating); // add the <user,rating> pair into movie list
		UserDictionary* u_dict=UserDictionary::getInstance();
		if (u_dict->existUser(user_id)){ // when the user id already exists
			u_dict->getUser(user_id)->add(mid,rating); // add the <movie, rating> pair into user list
		}else{ // when the user id does not exists
			_user_count++;// STATISTICS_OP: +1 user
			UserList* ul=new UserList(user_id); 
			ul->add(mid,rating); 
			u_dict->addUser(user_id,ul);
		}
		_pair_count++; //STATISTICS_OP: +1 pair
		_movie_rated_count[rating-1]++; //STATISTICS_OP: +1 count of rating
	}
}	