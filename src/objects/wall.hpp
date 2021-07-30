#ifndef WALL_HEADER
#define WALL_HEADER

#include "thing.hpp"

struct Wall: public Thing {
    
    Wall ();
    ~Wall ();
    
    static void tick (uint64_t tick_time);
    static uint16_t push (Thing* pusher, uint16_t dist, uint16_t dir);
    
};

#endif
