
#ifndef CONTOLLABLE_HEADER
#define CONTOLLABLE_HEADER

#include <vector>

#include "../ivec.hpp"
#include "thing.hpp"

class Controllable : virtual public Thing {
  public:
    std::function<void(Controllable* that, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos)> action;
    
    std::vector<Chunk*> get_active_chunks (const int64_t range);
};


#endif // CONTOLLABLE_HEADER
