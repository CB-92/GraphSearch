CXX		= g++ -std=c++17
ICC		= icc -std=c++17

CXXFLAGS  	= -Wall

LDFLAGS 	=  -lpthread
OPTFLAGS	=  -O3 -finline-functions
OTHERFLAGS	=  -L/usr/X11R6/lib -lm

FF_ROOT		= fastflow/ff

INCLUDES	= -I $(FF_ROOT) 


TARGETS = \
	sBFS \
	tBFS \
	fBFS

.PHONY: clean cleanall all
.SUFFIXES: .cpp 

all: $(TARGETS)

sBFS : sequential/sequentialBFS.cpp ./*hpp
	$(CXX) $< $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o sBFS

sIBFS : sequential/sequentialBFS.cpp ./*hpp
	$(ICC) $< $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o sIBFS

tBFS : thread/* ./*hpp
	$(CXX) thread/threadBFS.cpp $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o tBFS

tIBFS : thread/* ./*hpp
	$(ICC) thread/threadBFS.cpp $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) -o tIBFS

fBFS : fastflow/* ./*hpp
	$(CXX) fastflow/fastflowBFS.cpp $(CXXFLAGS) $(OPTFLAGS) $(OTHERFLAGS) $(LDFLAGS) $(INCLUDES) -o fBFS

clean		: 
	rm -f $(TARGETS) 
cleanall	: clean
	\rm -f *.o *~