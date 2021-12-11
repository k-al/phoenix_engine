FILES = thing.o loader.o visible.o controllable.o

build: $(FILES)

thing_hpp = thing.hpp ../ivec.hpp ../defs.hpp
thing.o: $(thing_hpp) thing.cpp ../server/chunk.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o thing.o thing.cpp
	cp thing.o ..

loader_hpp = loader.hpp $(thing_hpp)
loader.o: $(loader_hpp) loader.cpp ../server/*
	g++ $(CFLAGS) $(INCLUDES) -c -o loader.o loader.cpp
	cp loader.o ..

controllable_hpp = controllable.hpp $(loader_hpp)
controllable.o: $(controllable_hpp) controllable.cpp
	g++ $(CFLAGS) $(INCLUDES) -c -o controllable.o controllable.cpp
	cp controllable.o ..


visible_hpp = visible.hpp $(thing_hpp)
visible.o: $(visible_hpp) visible.cpp ../client/texture_manager.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o visible.o visible.cpp
	cp visible.o ..
