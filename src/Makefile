FILES = ivec.o main.o
export INCLUDES = $(LIB_INCLUDES) -I$(shell pwd)



build: $(FILES) build-server build-objects build-client

ivec.o: ivec.cpp ivec.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o ivec.o ivec.cpp

main.o: main.cpp server/server.hpp objects/wall.hpp client/client.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o main.o main.cpp

build-client:
	cd client && $(MAKE) build

clean-client:
	cd client && $(MAKE) clean

build-server: ivec.hpp
	cd server && $(MAKE) build

clean-server:
	cd server && $(MAKE) clean

build-objects: 
	cd objects && $(MAKE) build

clean-objects:
	cd objects && $(MAKE) clean

clean: clean-server clean-objects clean-client
	rm *.o
