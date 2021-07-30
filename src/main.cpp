#include <iostream>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <atomic>
// #include <SFML/Graphics.hpp>

#include "ivec.hpp"
#include "server/server.hpp"
#include "objects/wall.hpp"

int main() {
    std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " is here\n";
    Server myfirstserver = Server();
    Server_ini sv_ini;
    sv_ini.cycletime = std::chrono::milliseconds(50);
    myfirstserver.ini(sv_ini);
    
    myfirstserver.start();
    
    for (uint64_t i = 0; i < 2; ++i) {
        myfirstserver.load(iVec2(2, i));
        myfirstserver.generate(iVec2(2, i));
        myfirstserver.wake(iVec2(2, i));
    }
    
    Wall entity = Wall();
    
    myfirstserver.chunks[iVec2(2, 0)].add(&entity);
    
    std::this_thread::sleep_for (std::chrono::milliseconds(50));
    
    for (uint64_t i = 20000; i < 30000; ++i) {
        myfirstserver.sleep(iVec2(2, i));
    }
    
    
    std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " have slept and unload 1\n";
    std::this_thread::sleep_for (std::chrono::milliseconds(200));
    
    
    
//     std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " have slept and unload 2\n";
    return 0;
    
    
}
