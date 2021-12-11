// header guard
#ifndef CONTOLLABLE_HEADER
#define CONTOLLABLE_HEADER

#include "loader.hpp"

#include <vector>

class Controllable : virtual public Loader {
public:
    
    std::vector<Chunk*> active_chunks;
    
    // provoke actions based on user input (buttons, move direction, cursor position)
    virtual void action (std::vector<bool>, iVec2, iVec2);
    
    // get the pointer to the active Chunks on the Vector, but also load the Chunks, that are not active
    void get_active_chunks (std::vector<Chunk*>&, const uint32);
    
    // if the range is omitted the chunks are likely precomputed and just switched to the vector
    // if not precomputed, the function returns false (the buffervector is not overwritten in that case)
    bool get_active_chunks (std::vector<Chunk*>&);
    
    // override the standart load function to save the active chunks if the standart range is loaded
    virtual void load (uint32 range) override;
};


#endif
