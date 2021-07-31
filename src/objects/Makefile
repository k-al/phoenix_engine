FILES = thing.o wall.o

build: $(FILES)

thing.o: thing.cpp thing.hpp ../ivec.hpp ../server/server.hpp ../server/chunk.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o thing.o thing.cpp
	cp thing.o ..

wall.o: wall.cpp wall.hpp thing.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o wall.o wall.cpp
	cp wall.o ..

clean:
	rm *.o
