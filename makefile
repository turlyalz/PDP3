compile: 
	g++ -std=c++14 -Wall -pedantic -fopenmp -O3 src/*.cpp -o bw.out

valrun:
	valgrind ./bw.out

clean:
	rm -f ./bw.out
