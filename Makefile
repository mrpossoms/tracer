NAME=tracer

CXXC=g++
CXX_FLAGS=-g -ggdb -std=c++11 -Wno-deprecated-declarations

EXT=./external

# use the findingds of the findlibs.sh script
include findings.mk

SRC=tracer.cpp sphere.cpp viewer.cpp list_scene.cpp materials.cpp rand.cpp plane.cpp
OUT=./obj
OBJ = $(addprefix $(OUT)/,$(notdir $(SRC:.cpp=.o)))

LINK+=-lncurses

# inter-project includes
INC +=-I$(EXT)/linmath.h

all: $(OBJ)
	$(CXXC) $(CXX_FLAGS) $(INC) $(LIB_PATHS) src/main.cpp -o $(NAME) $(LINK) $(OBJ)

$(OUT)/%.o: src/%.cpp | $(OUT)
	$(CXXC) -o $@ -c $^ $(INC) $(CXX_FLAGS)

$(OUT):
	mkdir $(OUT)

clean:
	rm $(OBJ) || true
	rm $(OUT) || true

external:
	gitman install

findings.mk: external
	./findlibs.sh
