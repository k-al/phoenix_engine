// header guard
#ifndef LOADER_HEADER
#define LOADER_HEADER

#include "thing.hpp"

#include <vector>

class Chunk;

class Loader : virtual public Thing {
    int32 range;
    
    std::vector<Chunk*> active_chunks;
    
    void load (int32 range = -1);
}

#endif
