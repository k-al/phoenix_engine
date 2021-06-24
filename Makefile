CFLAGS = -std=c++17 -O3 -g
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

O_DEP = src/main.o src/ivec.o src/chunk.o src/plan.o src/server.o src/loader.o src/thing.o

Phoenix: $(O_DEP)
	##! WARNING: pthread may cause problems with compiling for Windows
	g++ $(CFLAGS) -o Phoenix $(O_DEP) -pthread $(SFMLFLAGS)

src/main.o: src/main.cpp src/ivec.hpp src/server.hpp
	g++ $(CFLAGS) -c -o src/main.o src/main.cpp

src/ivec.o: src/ivec.cpp src/ivec.hpp
	g++ $(CFLAGS) -c -o src/ivec.o src/ivec.cpp

src/chunk.o: src/chunk.cpp src/chunk.hpp src/thing.hpp src/ivec.hpp
	g++ $(CFLAGS) -c -o src/chunk.o src/chunk.cpp

src/plan.o: src/plan.cpp src/plan.hpp src/chunk.hpp src/ivec.hpp
	g++ $(CFLAGS) -c -o src/plan.o src/plan.cpp

src/server.o: src/server.cpp src/server.hpp src/loader.hpp src/plan.hpp src/chunk.hpp src/multh_listworker.hpp
	g++ $(CFLAGS) -c -o src/server.o src/server.cpp

src/loader.o: src/loader.cpp src/loader.hpp src/ivec.hpp src/plan.hpp src/chunk.hpp src/thing.hpp src/multh_listworker.hpp
	g++ $(CFLAGS) -c -o src/loader.o src/loader.cpp

src/thing.o: src/thing.cpp src/thing.hpp src/ivec.hpp
	g++ $(CFLAGS) -c -o src/thing.o src/thing.cpp

run: Phoenix
	./Phoenix

clean:
	rm -f src/*.o
	rm -f Phoenix
