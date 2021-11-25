
#ifndef CONTOLLABLE_HEADER
#define CONTOLLABLE_HEADER

#include <vector>

#include "../ivec.hpp"
#include "thing.hpp"

class Controllable {
  public:
    std::function<void(Controllable* that, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos)> action;
    
    std::vector<Chunk*> get_active_chunks (const int64_t range) {
        
        if (!(Thing* that = dynamic_cast<Thing*>(this)))
            return std::vector<Chunk*>();
        
        std::vector<Chunk*> res;
        
        iVec2 dif;
        auto chunk_it = that->server->chunks.end();
        
        for (dif.x = -range; dif.x <= range; ++dif.x) {
            for (dif.y = this->load_range; dif.y <= this->load_range; ++dif.y) {
                // find Chunk in chunk_it->second
                chunk_it = this->server->chunks.find(this->chunk->position + dif);
                // tests, if chunk is an active chunk
                if (chunk_it != that->server->chunks.end() && !chunk_it->second.is_active()) {
                    res.push_back(&chunk_it->second);
                }
            }
        }
    }
};


#endif // CONTOLLABLE_HEADER
