OUT=chimera

CXXC=g++
CXX_FLAGS=-g -ggdb -std=c++11 -Wno-deprecated-declarations

EXT=./external

# use the findingds of the findlibs.sh script
include findings.mk

SRC=tracer.cpp
OUT=./obj
OBJ = $(addprefix $(OUT)/,$(notdir $(SRC:.cpp=.o)))

SRC_VISUAL=$(SRC)/visual
SRC_GL=$(SRC_VISUAL)/GL_1_3

# inter-project includes
INC +=-I$(EXT)/linmath.h

.PHONY:
what:
	echo $(SRC)
	echo $(OBJ)

$(OUT)/%.o: src/%.cpp | $(OUT)
	$(CXXC) -o $@ -c $^ $(INC) $(CXX_FLAGS)

$(OUT): findings.mk
	mkdir $(OUT)


all: findings.mk
	$(CXXC) $(CXX_FLAGS) $(INC_PATHS) $(LIB_PATHS) $(SRC)/main.cpp -o $(OUT) $(LINK) $(OBJ)/*.o

clean: clear-data
	rm $(OBJ)/*.o || true
	rm $(OUT) || true

external:
	gitman install

findings.mk: external
	./findlibs.sh
