##
##

CXX = g++ 
CXXFLAGS = -std=c++11

run: Map.cpp GameSetup.cpp City.cpp main.cpp Helpers.cpp Map.h GameSetup.hpp City.h Helpers.hpp
	$(CXX) $(CXXFLAGS) Map.cpp GameSetup.cpp City.cpp main.cpp Helpers.cpp
	./a.out
	
clean:
	-rm -f game
	-rm -f a.out