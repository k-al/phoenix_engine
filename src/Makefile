FILES = ivec.o main.o
export INCLUDES = $(LIB_INCLUDES) -I$(shell pwd)



build: $(FILES) build-server build-objects

ivec.o: ivec.cpp ivec.hpp
	g++ $(CFLAGS) $(INCLUDES) -c -o ivec.o ivec.cpp

main.o: main.cpp
	g++ $(CFLAGS) $(INCLUDES) -c -o main.o main.cpp

build-server: ivec.hpp
	cd server && $(MAKE) build

clean-server:
	cd server && $(MAKE) clean

build-objects: 
	cd objects && $(MAKE) build

clean-objects:
	cd objects && $(MAKE) clean

clean: clean-server clean-objects
	rm *.o
