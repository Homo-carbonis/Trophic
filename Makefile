all : libtrophic.so
libtrophic.so : trophic.o
	g++ -shared -fpic -std=c++20 -lGL -lGLEW -lSDL2 -lpng -o libtrophic.so trophic.o
trophic.o : trophic.cpp trophic.hpp graphics.hpp contiguous_map.hpp uniform.hpp matrixTransform.hpp
	g++ -c -fpic -std=c++20 -o trophic.o trophic.cpp
clean :
	rm trophic.o libtrophic.so
