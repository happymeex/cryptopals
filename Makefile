set1: test/set1.cpp build/hex.o
	g++ -std=c++20 $^ -o build/set1

test: test/aes_t.cpp build/aes.o build/hex.o build/test.o
	g++ -std=c++20 $^ -o build/test

build/test.o: test/test.cpp
	g++ -std=c++20 -c $^ -o $@

build/hex.o: src/Hex.cpp
	g++ -std=c++20 -c $^ -o $@
	
build/aes.o: src/aes.cpp
	g++ -std=c++20 -c $^ -o $@
