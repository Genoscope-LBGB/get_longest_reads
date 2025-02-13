all: build

build: get_longest.cpp get_longest.hpp
	g++ -o get_longest -std=c++11 -O3 -Wall -pedantic -Wno-unused-variable -Wno-unused-but-set-variable get_longest.cpp
