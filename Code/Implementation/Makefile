# Makefile for Movie Recommendation--- HW5 of Search Engine 11-641
#   Yitong Zhou
#   Nov 19, 2012

#source code directory:
src=src
#obj code directory:
obj=obj
ValFlag=-g -O2

files=src/main.cpp src/CorpusExp.cpp src/MovieList.cpp src/MovieDictionary.cpp src/UserDictionary.cpp src/UserList.cpp src/SimCache.cpp src/SimCalculator.cpp src/MovieMovieSim.cpp src/UserUserSimNorm.cpp src/MovieMovieSimNorm.cpp src/MySim.cpp
recommend: $(files)
	g++ $(ValFlag) $(Flag) $(files) -o recommend

clean:
	rm -rf obj/*.o recommend
