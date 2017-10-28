NAME=tracer
INSTALL_DIR_INC=/usr/include/tracer
INSTALL_DIR_LIB=/usr/lib

CXXC=g++
CXX_FLAGS=-g -ggdb -std=c++11 -Wno-deprecated-declarations -lpthread

EXT=./external

# use the findingds of the findlibs.sh script
include findings.mk

SRC=tracer.cpp sphere.cpp viewer.cpp list_scene.cpp materials.cpp rand.cpp plane.cpp
OUT=./obj
OBJ = $(addprefix $(OUT)/,$(notdir $(SRC:.cpp=.o)))

LINK+=-lncurses -lpthread

# inter-project includes
INC +=-I$(EXT)/linmath.h

all: $(OBJ)
	$(CXXC) $(CXX_FLAGS) $(INC) $(LIB_PATHS) src/main.cpp -o $(NAME) $(LINK) $(OBJ)

lib: $(OBJ)
	ar rcs lib$(NAME).a $(OBJ)

install: $(INSTALL_DIR_INC)
	cp src/*.h $(INSTALL_DIR_INC)
	cp lib$(NAME).a $(INSTALL_DIR_LIB)

$(INSTALL_DIR_INC):
	mkdir $(INSTALL_DIR_INC)

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
