FILES = client.o interaction.o ui.o

build: $(FILES)

client.o: client.cpp client.hpp ../ivec.hpp ../server/server.hpp ../objects/thing.hpp ../objects/controllable.hpp ui.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o client.o client.cpp
	cp client.o ..

interaction.o: interaction.cpp interaction.hpp ../ivec.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o interaction.o interaction.cpp
	cp interaction.o ..

ui.o: ui.cpp ui.hpp ../ivec.hpp ../objects/controllable.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o ui.o ui.cpp
	cp ui.o ..

clean:
	rm *.o
