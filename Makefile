CXXFLAGS ?= " -march=native -mtune=native -pedantic -Wno-unused-variable -Wno-unused-but-set-variable "

all: build

build: get_longest.cpp get_longest.hpp
	g++ -o get_longest -std=c++11 -O3 -Wall $(CFLAGS) get_longest.cpp
