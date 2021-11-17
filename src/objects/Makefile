FILES = thing.o wall.o visible.o player.o

build: $(FILES)

thing.o: thing.cpp thing.hpp ../ivec.hpp ../server/server.hpp ../server/chunk.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o thing.o thing.cpp
	cp thing.o ..

wall.o: wall.cpp wall.hpp visible.hpp thing.hpp ../ivec.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o wall.o wall.cpp
	cp wall.o ..

visible.o: visible.cpp visible.hpp thing.hpp ../ivec.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o visible.o visible.cpp
	cp visible.o ..

player.o: player.cpp player.hpp visible.hpp controllable.hpp ../client/interaction.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o player.o player.cpp

clean:
	rm *.o
