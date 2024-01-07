CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17
LDFLAGS = $(shell pkg-config --cflags --libs gtest)

SRC_CONTROLLER = $(wildcard Controller/*.cpp)
HEADER_CONTROLLER = $(patsubst %.cpp, %.h, ${SRC_CONTROLLER})

SRC_VIEW = $(wildcard View/*.cpp)
HEADER_VIEW = $(wildcard View/*.h)

SRC_MODEL = $(wildcard Model/*.cpp Model/Maze/*.cpp Model/Cave/*.cpp)
HEADER_MODEL = $(patsubst %.cpp, %.h, ${SRC_MODEL})

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	OPEN = open build/maze.app
else
	OPEN = ./build/maze
endif

all: install

install:
	@mkdir build/ && cd build/ && qmake ../maze.pro && make
	$(OPEN)

open:
	$(OPEN)

uninstall:
	rm -rf build/

dist:
	tar -cf ../A1_Maze.tar Controller/ View/ Model/ examples/ main.cpp maze.pro Makefile

clean:
	rm -rf build/
	rm -rf ../*.tar

.PHONY: all install uninstall open dist clean