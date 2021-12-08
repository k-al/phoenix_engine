
#include "thing.hpp"

#include "../server/chunk.hpp"

inline bool border_check (Thing* that) {
    
    constexpr int32 mask = ~Chunk::coo_max;
    
    int32 x = that->pos.x & mask;
    int32 y = that->pos.y & mask;
    
    if (x | y == 0) // no overflow
        return false;
    
    //! TODO
    // that->chunk->pipeline_chunkchange();
    
    return true;
}

bool Thing::move (iVec2 dif) {
    this->pos += dif;
    return border_check(this);
    
}

void Thing::teleport (iVec2 new_pos) {
    this->pos = new_pos;
    border_check(this);
}
