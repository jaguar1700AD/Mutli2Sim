all: tester lib

tester: main.cc
	g++ -c -o main.o main.cc
	g++ -o main main.o lut.a

lib: lut.cc lut.h
	g++ -c -o lut.o lut.cc --std=c++11
	ar rcs lut.a lut.o
	g++ -c -o main.o main.cc
	g++ -o main main.o lut.a

clean: rm *.o *.a main
