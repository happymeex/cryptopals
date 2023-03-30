set1: test/set1.cpp build/hex.o
	g++ -std=c++20 $^ -o build/set1

build/test: build/test.o build/hex.o
	g++ -std=c++20 $^ -o $@

build/test.o: test/test.cpp
	g++ -std=c++20 -c $^ -o $@

build/hex.o: src/Hex.cpp
	g++ -std=c++20 -c $^ -o $@
	

