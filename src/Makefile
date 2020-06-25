linux:
	g++ -g pixel.cpp `sdl2-config --cflags --libs`

windows:
	g++ -g -std=c++11 pixel.cpp "-Wl,-subsystem,windows" -lmingw32 -lSDL2main -lSDL2 -o pixel.exe
