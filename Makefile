default: build

build:
	mpic++ -Wall -W -s -O3 -march=native -std=c++11 *.cpp -o program

clean:
	rm -f *.o program outFile*

run:
	mpiexec -np $(n) program
