
#include "thing.hpp"
#include "wall.hpp"


// Walls dont need to do anything
void Wall::tick(uint64_t tick_time) {
    return;
}

// Walls cannot be pushed
uint16_t Wall::push (Thing* pusher, uint16_t dist, uint16_t dir) {
    return 0;
}
