SRC = $(wildcard src/*.cpp)

CC = g++

COMPILER_FLGAS = -w -Wall -O -pg
INCLUDE = -Iinclude/

LINKER_FLAGS = -lSDL2 -lSDL2_image

OBJ_NAME = game

all: $(OBJS)
	 $(CC) $(SRC) $(COMPILER_FLGAS) $(INCLUDE) $(LINKER_FLAGS) -o $(OBJ_NAME)
