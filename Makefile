CXX		= g++ -std=c++17
ICC		= icc -std=c++17

CXXFLAGS  	= -Wall

LDFLAGS 	=  -lpthread
OPTFLAGS	=  -O3 -finline-functions
OTHERFLAGS	=  -L/usr/X11R6/lib -lm

TARGETS = \
	sBFS \
	sIBFS 

.PHONY: clean cleanall
.SUFFIXES: .cpp 


sBFS : sequential/sequentialBFS.cpp
	$(CXX) $< $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o sBFS

sIBFS : sequential/sequentialBFS.cpp
	$(ICC) $< $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o sIBFS

clean		: 
	rm -f $(TARGETS) 
cleanall	: clean
	\rm -f *.o *~