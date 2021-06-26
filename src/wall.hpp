#ifndef WALL_HEADER
#define WALL_HEADER

#include "thing.hpp"

struct Wall: Thing {
    
    
    void tick (uint64_t tick_time);
    uint16_t push (Thing* pusher, uint16_t dist, uint16_t dir);
    
};

#endif
