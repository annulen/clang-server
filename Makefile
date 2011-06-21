CC ?= gcc
CXX ?= g++
CFLAGS ?= -O2
CXXFLAGS ?= $(CFLAGS)

all: clang-server clang-client clang-make

clang-server: clang-server.cpp
	$(CXX) $(CXXFLAGS) clang-server.cpp -o $@

clang-client: clang-server.cpp
	$(CXX) $(CXXFLAGS) clang-client.cpp -o $@

clang-make: clang-make.c
	$(CC) $(CFLAGS) clang-make.c -o $@

clean:
	rm -f clang-server clang-client clang-make clang-server.o clang-client.o clang-make.o
