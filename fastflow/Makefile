CXX		= g++ -std=c++17
ICC		= icc -std=c++17

CXXFLAGS  	= -Wall -DBLOCKING_MODE -DNO_DEFAULT_MAPPING 

LDFLAGS 	=  -lpthread
OPTFLAGS	=  -O3 -finline-functions -DNDEBUG 
OTHERFLAGS	=  -L/usr/X11R6/lib -lm

FF_ROOT		= ./ff

INCLUDES	= -I $(FF_ROOT) 


TARGETS = \
	BFS \
	IBFS 

.PHONY: BFS IBFS clean cleanall
.SUFFIXES: .cpp 

BFS : BFS_Fast_Flow.cpp
	$(CXX) $< $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o BFS

IBFS : BFS_Fast_Flow.cpp
	$(ICC) $< $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o IBFS

clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~
