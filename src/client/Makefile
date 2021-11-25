FILES = client.o

build: $(FILES)

client.o: client.cpp client.hpp ../ivec.hpp ../server/server.hpp ../objects/thing.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o client.o client.cpp
	cp client.o ..

interaction.o: interaction.cpp interaction.hpp ../ivec.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o interaction.o interaction.cpp
	cp interaction.o ..

clean:
	rm *.o
