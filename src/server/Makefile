FILES = server.o chunk.o

build: $(FILES)

server.o: server.cpp server.hpp chunk.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o server.o server.cpp
	cp server.o ..

chunk.o: chunk.cpp chunk.hpp ../objects/thing.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o chunk.o chunk.cpp
	cp chunk.o ..

clean:
	rm *.o
