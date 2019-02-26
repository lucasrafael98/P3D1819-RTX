
default:
	g++ -c camera.cpp
	g++ -c color.cpp
	g++ -c ray.cpp
	g++ -c scene.cpp
	g++ -c main.cpp
	g++ -o rtx camera.o color.o ray.o scene.o main.o -lGL -lGLEW -lglut -lGLU

clean:
	rm *.o rtx
