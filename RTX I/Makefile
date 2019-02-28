
default:
	g++ -c Vector3.cpp
	g++ -c Color.cpp
	g++ -c Ray.cpp
	g++ -c Light.cpp
	g++ -c Material.cpp
	g++ -c Camera.cpp
	g++ -c Scene.cpp
	g++ -c main.cpp
	g++ -o bin/rtx Vector3.o Color.o Ray.o Light.o Material.o Camera.o Scene.o main.o -lGL -lGLEW -lglut -lGLU
	rm *.o

clean:
	rm *.o bin/rtx
