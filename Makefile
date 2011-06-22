CC ?= gcc
CXX ?= g++
CFLAGS ?= -O2
CXXFLAGS ?= $(CFLAGS)
PREFIX ?= /usr/local

all: clang-server clang-client clang++-client clang-make

clang-server: clang-server.cpp
	$(CXX) $(CXXFLAGS) clang-server.cpp -o $@

clang-client: clang-client.cpp
	$(CXX) $(CXXFLAGS) clang-client.cpp -o $@

clang++-client:
	ln -s clang-client $@

clang-make: clang-make.cpp
	$(CXX) $(CXXFLAGS) clang-make.cpp -o $@

install:
	install clang-server clang-client clang++-client clang-make $(PREFIX)

clean:
	rm -f clang-server clang-client clang++-client clang-make clang-server.o clang-client.o clang-make.o