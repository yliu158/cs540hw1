run: test.cpp
	g++ -std=c++11 test.cpp -o test -ldl
clean:
	rm -fr test
