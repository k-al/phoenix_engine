FILES = thing.o

build: $(FILES)

thing_hpp = thing.hpp ../ivec.hpp
thing.o: $(thing_hpp) thing.cpp ../server/chunk.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o thing.o thing.cpp
	cp thing.o ..
