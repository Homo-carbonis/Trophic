CXX=g++
.PHONY: all lib
all : lib trophic

lib: libtrophic.so

trophic: main.cpp libtrophic.so
	$(CXX) -g -std=c++20 -lGL -lGLEW -lSDL2 -lpng -o $@ $<

libtrophic.so : libtrophic.o
	$(CXX) -g -shared -fpic -std=c++20 -lGL -lGLEW -lSDL2 -lpng -o $@ $<

libtrophic.o : trophic.cpp trophic.hpp graphics.hpp contiguous_map.hpp uniform.hpp reactive/reactive.hpp reactive/transform.hpp reactive/inputs.hpp 
	$(CXX) -g -c -fpic -std=c++20 -o $@ $<

clean :
	rm trophic libtrophic.o libtrophic.so

install :
	cp libtrophic.so /usr/local/lib/
	mkdir -p /usr/local/include/trophic
	cp *.hpp /usr/local/include/trophic/
