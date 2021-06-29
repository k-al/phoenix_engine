
CFLAGS = -std=c++17 -O3 -g

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

O_DEP = device.o window_handler.o window.o

device.o: device.cpp device.hpp
	g++ $(CFLAGS) -c -o device.o device.cpp

window_handler.o: window_handler.cpp window_handler.hpp
	g++ $(CFLAGS) -c -o window_handler.o window_handler.cpp

window.o: window.cpp window.hpp device.hpp window_handler.hpp
	g++ $(CFLAGS) -c -o window.o window.cpp

compiler_test: $(O_DEP)

test: $(O_DEP) test.cpp
	g++ $(CFLAGS) -c -o test.o test.cpp
	g++ $(CFLAGS) -o Test.app test.o $(O_DEP) $(LDFLAGS)
	./Test.app

clean:
	rm -f ./*.app
	rm -f ./*.o
