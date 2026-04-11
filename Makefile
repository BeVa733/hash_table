CXX := g++

MAIN := main.cpp
RESEARCH := hash_research

SRC := $(MAIN) hash_table.cpp hash_funcs.cpp onegin.cpp spisok.cpp dump.cpp

COMMON_FLAGS := -Wall -Wextra -Wshadow -O3 -DNDEBUG 

all: $(RESEARCH)

$(RESEARCH): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(RESEARCH)

plot:
	python3 plot_hist.py

clean:
	rm -f $(RESEARCH) *.o *.csv *.png *.html test1.txt 

.PHONY: all debug release plot clean