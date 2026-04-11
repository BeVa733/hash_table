CXX := g++

CXXFLAGS := -Wall -Wextra -Wshadow -O3 -g \
            -I./include \
            -I./soa_list/include \
            -DLIST_TYPE='const char*' \
            -DPRINT_SPEC='"s"'

TARGET := hash_research

SRC := src/research_main.cpp \
       src/hash_table.cpp \
       src/hash_funcs.cpp \
       read_modul/onegin.cpp \
       soa_list/src/spisok.cpp \
       soa_list/src/dump.cpp

OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET).out

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) *.csv *.png *.html *.out

remake: clean all

.PHONY: all clean remake