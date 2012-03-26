include make.inc

default: all

all: classifymain.o
	$(CP) classify.sh.pro $(CUTER)/build/prototypes/classify.sh.pro
	sed -f $(MYCUTER)/double/config/script.sed classify.sh.pro > $(MYCUTER)/bin/classify
	chmod a+x $(MYCUTER)/bin/classify
	$(CP) classifymain.cpp $(MYCUTER)/double/bin/classifyma.cpp
	$(MV) classifymain.o $(MYCUTER)/double/bin/classifyma.o

%.o: %.cpp
	$(CXX) -c -o $@ $< $(DCII) $(CXXFLAGS) -I$(CUTER)/common/include

clean:
	rm -f classifymain.o
