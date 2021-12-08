
#include "../server/server.hpp"

#include "controllable.hpp"

// #include <thread> // just for debugging
// #include <iostream>

std::vector<Chunk*> Controllable::get_active_chunks (const int64_t range) {
    std::vector<Chunk*> res;
    
    iVec2 dif;
    auto chunk_it = this->server->chunks.end();
    
    for (dif.x = -range; dif.x <= range; ++dif.x) {
        for (dif.y = -range; dif.y <= range; ++dif.y) {
            // find Chunk in chunk_it->second
            chunk_it = this->server->chunks.find(this->chunk->position + dif);
            
            // tests, if chunk is an active chunk
            if (chunk_it != this->server->chunks.end() && chunk_it->second.is_active()) {
                res.push_back(&chunk_it->second);
            }
            
            
//             // debugging:
//             std::cout << std::dec << "(" << dif.x << "|" << dif.y << ") -> (" << (this->chunk->position + dif).x << "|" << (this->chunk->position + dif).y << ") ";
//             if (chunk_it != this->server->chunks.end()) {
//                 if (chunk_it->second.is_active()) {
//                     std::cout << "active!\n";);
//                 } else {
//                     std::cout << "inactive\n";
//                 }
//             } else {
//                 std::cout << "nonexistent\n";
//             }
            
        }
    }
    
    return res;
}
