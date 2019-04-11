GLFLAGS=-lGL -lGLEW -lglut -lGLU 

default:
	mkdir -p bin
	for cppfile in *.cpp; do\
		g++ -ansi -std=c++17 -Wall -pedantic -c $$cppfile;\
	done
	g++ -o bin/rtx *.o ${GLFLAGS}
	rm *.o

clean:
	rm -rf bin/
