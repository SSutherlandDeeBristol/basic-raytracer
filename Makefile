
SDL_FLAGS = `sdl2-config --cflags --libs`
CC = g++
CC_FLAGS = -std=c++17 -O2

FILE = test

build:
	$(CC) $(CC_FLAGS) $(SDL_FLAGS) $(FILE).cpp -o $(FILE)

run: build
	./$(FILE)

clean:
	rm $(FILE)