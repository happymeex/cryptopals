FLAGS = -std=c++20

#build/set1.o: test/set1.cpp build/hex.o build/aes.o build/utils.o
#	g++ $(FLAGS) $^ -o $@
#
#test: test/aes_t.cpp build/aes.o build/hex.o build/test.o
#	g++ $(FLAGS) $^ -o build/test
#
#test_set1: test/test_set1.cpp build/hex.o build/set1.o
#	g++ $(FLAGS) $^ -o build/test_set1
#
#build/test.o: test/test.cpp
#	g++ $(FLAGS) -c $^ -o $@
#
build/hex.o: src/hex.cpp
	g++ $(FLAGS) -c $^ -o $@
	
build/aes.o: src/aes.cpp
	g++ $(FLAGS) -c $^ -o $@

build/utils.o: src/utils.cpp
	g++ $(FLAGS) -c $^ -o $@

build/set1.o: src/set1.cpp
	g++ $(FLAGS) -c $^ -o $@

build/bseq.o: src/bseq.cpp src/bseq.hpp
	g++ $(FLAGS) -c $< -o $@

test_utils: test/test_utils.cpp build/bseq.o build/utils.o
	g++ $(FLAGS) $^ -o build/test_utils

test_set1: test/test_set1.cpp build/bseq.o build/set1.o build/aes.o build/utils.o
	g++ $(FLAGS) $^ -o build/test_set1

test_set2: test/test_set2.cpp build/bseq.o
	g++ $(FLAGS) $^ -o build/test_set2