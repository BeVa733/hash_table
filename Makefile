CXX := g++
OBJ_DIR := obj

CXXFLAGS := -Wall -Wextra -Wshadow -O3 -g \
            -I./hash_table/include \
            -I./soa_list/include \
            -march=native \
			-DFINAL_VER \
            -DNDEBUG \
            -DLIST_TYPE='const char*' \
            -DPRINT_SPEC='"s"'

COMMON_SRC := src/hash_table.cpp \
              src/hash_funcs.cpp \
              read_modul/onegin.cpp \
              soa_list/src/spisok.cpp \
              soa_list/src/dump.cpp

COMMON_OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(COMMON_SRC:.cpp=.o)))

TARGET1 := hash_research
MAIN1   := src/research_main.cpp
OBJ1    := $(OBJ_DIR)/$(notdir $(MAIN1:.cpp=.o))

TARGET2 := hash_opt
MAIN2   := src/optimization_main.cpp
OBJ2    := $(OBJ_DIR)/$(notdir $(MAIN2:.cpp=.o))

VPATH := hash_table/src:read_modul:soa_list/src

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(COMMON_OBJ) $(OBJ1)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET1).out

$(TARGET2): $(COMMON_OBJ) $(OBJ2)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET2).out

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) *.out

remake: clean all

.PHONY: all clean remake