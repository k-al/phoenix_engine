// header guard
#ifndef THING_HEADER
#define THING_HEADER

#include "../ivec.hpp"
#include "../defs.hpp"

class Chunk;

class Thing {
public:
    Chunk* chunk;
    iVec2 pos;
    
    iVec2 hitbox; // Vector half the size of the hitbox (for fast computing the corners)
    
    Thing () {}
    virtual ~Thing () {}
    
    ////////////////////////////////////////////////
    // Basic movements:
    
    // no collision checking, only chunk border checking
    // returns true if a chunk relocation occures
    bool move (iVec2);
    
    // move (coords, chunks) for large distances
    // returns true if a chunk relocation occures (nearly always)
    bool move (iVec2, iVec2);
    
    // teleport inside one chunk (with border checks)
    void teleport (iVec2);
    
    // teleport to different chunks
    void teleport (iVec2, iVec2);
    void teleport (iVec2, Chunk*);
    
    // teleport to different planes (coords, chunk, plane)
    void teleport (iVec2, iVec2, uint16);
    
    ////////////////////////////////////////////////////
    // Hitbox management
    
    // resize the hitbox
    void resize (iVec2);
    
    // returns true if the hitboxes overlap
    bool overlap (Thing*);
    
    //////////////////////////////////////////////
    // Basic fallback functions:
    
    // returns true if Thing and this can't share the same space
    virtual bool is_solide (Thing*) { return false; }
    
    // code that is executed every tick
    virtual void tick (uint64) {}
};

#endif
