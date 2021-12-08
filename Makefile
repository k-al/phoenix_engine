export CFLAGS = -std=c++17 -O3 -g -Wall
export LIB_INCLUDES = -I$(shell pwd)/lib-multh/lib/
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system



Tarnish.app: build
	##! WARNING: pthread may cause problems with compiling for Windows !##
	g++ $(CFLAGS) $(LOCAL_INCLUDES) -o Tarnish.app src/*.o -pthread $(SFMLFLAGS)

build:
	echo $(LIB_INCLUDES)
	cd src && $(MAKE) build

clean:
	rm -f Tarnish.app
	cd src && $(MAKE) clean

run: Tarnish.app
	./Tarnish.app
