all:
	g++ -g pixel.cpp `sdl2-config --cflags --libs`
