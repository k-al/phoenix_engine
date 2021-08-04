FILES = client.o

build: $(FILES)

client.o: client.cpp client.hpp ../ivec.hpp ../server/server.hpp ../objects/thing.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o client.o client.cpp
	cp client.o ..

clean:
	rm *.o
