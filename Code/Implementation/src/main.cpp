using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "CorpusExp.h"
#include "SimCalculator.h"
#include "MovieMovieSim.h"
#include "UserUserSimNorm.h"
#include "MovieMovieSimNorm.h"
#include "MySim.h"

double diffclock(clock_t clock1,clock_t clock2){
	double diffticks=clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffticks;

}
int main(int argc, char* argv[]){
	clock_t t1,t2;
	t1=time(0);
	ifstream infile("../DATA.txt");
	string train_dir;   // The training data directory
	string url; // The query directory
	infile>>train_dir;
	infile>>url;
	train_dir="../"+train_dir.substr(6,train_dir.length());
	url="../"+url.substr(6,url.length());
	string ks;
	infile>>ks; // read the 3rd line, the k value
	ks=ks.substr(2,ks.length());  
	int k=atoi(ks.c_str()); 
	//LOAD all the corpus into memory:
	CorpusExp ce(train_dir,"all");
	ce.calculate();
	if (argc>1){
		if (strcmp(argv[1],"-s")==0){ // enter into CorpusExploration mode
			if (argc>=4 && strcmp(argv[2],"-u")==0){ // check user stats
				int user_id=atoi(argv[3]);
				ce.displayUser(cout,user_id);
			}
			else if (argc>=4 && strcmp(argv[2],"-m")==0){ // check movie stats
				int movie_id=atoi(argv[3]);
				ce.displayMovie(cout,movie_id);
			}else{ // check the all (default) stats
				ce.displayAll(cout);
			}
		}else if(strcmp(argv[1],"-m")==0) {// enter into Recommendation mode
			if (strcmp(argv[2],"1")==0){ // algorithm mode 1==> User-User Similarity
				SimCalculator algo1(url,k);
				ofstream ofs("../result_m_1.txt");
				algo1.calculate(ofs);
				cout<<"result_file: result_m_1.txt"<<endl;
			}else if (strcmp(argv[2],"2")==0){//algorithm mode 2==> Movie Movie Similarity
				MovieMovieSim algo2(url,k);
				ofstream ofs("../result_m_2.txt");
				algo2.calculate(ofs);
				cout<<"result_file: result_m_2.txt"<<endl;
			}else if (strcmp(argv[2],"3")==0){//algorithm mode 3==> User User Similarity with normalization
				//UserUserSimNorm algo3(url,200);
				MovieMovieSimNorm algo3(url,k);
				ofstream ofs("../result_m_3.txt");
				algo3.calculate(ofs);
				cout<<"result_file: result_m_3.txt"<<endl;
			}else if (strcmp(argv[2],"4")==0){//algorithm mode 4==> Self-made Algorithm
				MySim algo4(url,k);
				ofstream ofs("../result_m_4.txt");
				algo4.calculate(ofs);
				cout<<"result_file: result_m_4.txt"<<endl;
			}
		}else if(strcmp(argv[1],"-h")==0){
			cout<<"-s [none | -u USERID | -m MOVIEID]      Statistical Mode"<<endl;
			cout<<"-m [1-4]                                Calculation Model Select"<<endl;
		}
	}else{
			cout<<"-s [none | -u USERID | -m MOVIEID]      Statistical Mode"<<endl;
			cout<<"-m [1-4]                                Calculation Model Select"<<endl;
	}

	t2=time(0);
	double dif=diffclock(t2,t1);
	cout<<"Runtime:"<<dif<<"s"<<endl;
	return 0;
}
