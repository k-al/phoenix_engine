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
#include "objects/player.hpp"
#include "client/client.hpp"

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
    
    sf::Texture wall;
    wall.loadFromFile("textures/brick_wall32.png");
    
    sf::Texture mandrio;
    mandrio.loadFromFile("textures/mandrio.png");
    
    Wall entity = Wall();
    entity.sprite.setTexture(wall);
    
    myfirstserver.chunks[iVec2(2, 0)].add(&entity);
    
    Player player = Player();
    
    player.chunk = &myfirstserver.chunks[iVec2(2, 0)];
    player.server = &myfirstserver;
    player.sprite.setTexture(mandrio);
    
    myfirstserver.chunks[iVec2(2, 0)].add(&player);
    
    Client myfirstclient = Client();
    
    myfirstclient.change_follow(&player);
    
    myfirstclient.run();
    
//     std::this_thread::sleep_for (std::chrono::milliseconds(250));
    
    
//     std::cout << "Main Thread " << std::hex << std::this_thread::get_id() << " have slept and unload 2\n";
    
    std::cout << "Size:  " << std::dec << sizeof(Thing) << "\n";
    
    return 0;
    
    
}
