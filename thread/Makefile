CXX		= g++ -std=c++17
ICC		= icc -std=c++17

CXXFLAGS  	= -Wall

LDFLAGS 	=  -lpthread
OPTFLAGS	=  -O3 -finline-functions
OTHERFLAGS	=  -L/usr/X11R6/lib -lm

TARGETS = \
	BFS \
	IBFS 

.PHONY: BFS IBFS clean cleanall
.SUFFIXES: .cpp 


BFS : pthreadBFS.cpp
	$(CXX) $< $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o BFS

IBFS : pthreadBFS.cpp
	$(ICC) $< $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o IBFS

clean		: 
	rm -f $(TARGETS) 
cleanall	: clean
	\rm -f *.o *~