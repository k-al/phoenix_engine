
#include "thing.hpp"
#include "wall.hpp"

#include <iostream>

Wall::Wall () {
    this->tick_hook = this->tick;
    this->push_hook = this->push;
    this->is_solide = [](const Thing*)->bool{return true;}; // always collide with everything
}

Wall::~Wall () {}

// Walls dont need to do anything
void Wall::tick (uint64_t tick_time) {
    std::cout << "hey i got a tick!\n";
    return;
}

// Walls cannot be pushed
uint16_t Wall::push (Thing* pusher, uint16_t dist, uint16_t dir) {
    return 0;
}
