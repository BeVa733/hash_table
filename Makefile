CXX := g++

OBJ_DIR := obj

CXXFLAGS := -Wall -Wextra -Wshadow -O3 -g \
            -I./hash_table/include \
            -I./soa_list/include \
			-DFINAL_VER \
            -DNDEBUG \
            -DLIST_TYPE='const char*' \
            -DPRINT_SPEC='"s"'

TARGET := hash_research

SRC := hash_table/src/research_main.cpp \
       hash_table/src/hash_table.cpp \
       hash_table/src/hash_funcs.cpp \
       read_modul/onegin.cpp \
       soa_list/src/spisok.cpp \
       soa_list/src/dump.cpp    

OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

VPATH := hash_table/src:read_modul:soa_list/src

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET).out

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) *.csv  *.html *.out

remake: clean all

.PHONY: all clean remake