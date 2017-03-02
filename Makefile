OUT=chimera

CXXC=g++
CXX_FLAGS=-g -ggdb -std=c++11 -Wno-deprecated-declarations

EXT=./external

# use the findingds of the findlibs.sh script
include findings.mk

SRC=./src
OBJ=./obj

SRC_VISUAL=$(SRC)/visual
SRC_GL=$(SRC_VISUAL)/GL_1_3

# inter-project includes
INC_PATHS+=-I$(EXT)/linmath.h -I$(EXT)/opt.h -I$(SRC) -I$(SRC_VISUAL)


GLFW_LINK=-lglfw3
GL_LINK= -lpng

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	GL_LINK += $(GLFW_LINK) -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
endif
ifeq ($(UNAME),Linux)
	GL_LINK += -lGL -lglut -lGLU -lX11 -ldl -lpthread -lXxf86vm -lXrandr -lXi -lXinerama -lXcursor
	CXX_FLAGS+=-D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200112L
endif

LINK=$(GL_LINK)

all: findings.mk
	$(CXXC) $(CXX_FLAGS) $(INC_PATHS) $(LIB_PATHS) $(SRC)/main.cpp -o $(OUT) $(LINK) $(OBJ)/*.o

clean: clear-data
	rm $(OBJ)/*.o || true
	rm $(OUT) || true

findings.mk:
	./findlibs.sh
	make -C . all
