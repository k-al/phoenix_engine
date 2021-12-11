// header guard
#ifndef LOADER_HEADER
#define LOADER_HEADER

#include "thing.hpp"

class Chunk;

class Loader : virtual public Thing {
    int32 range;
    
    // if the range is omitted the default loading range is used
    virtual void load (uint32);
    inline void load () { this->load(this->range); }
}

#endif
