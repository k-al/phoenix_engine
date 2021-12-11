
#include "loader.hpp"

#include "../server/server.hpp"

void Loader::load (uint32 range) {
    
    Server* server = this->chunk->server;
    std::unordered_map<iVec2, Chunk, Server::MapKeyHasher>& chunks = server->chunks; // auto should compile to std::unordered_map<iVec2, Chunk, Server::MapKeyHasher>
    const iVec2 chunk_pos = this->chunk->position;
    
    auto chunk_it = chunks.end();
    iVec2 dif;
    
    for (dif.x = -this->range; dif.x <= this->range; ++dif.x) {
        for (dif.y = this->range; dif.y <= this->range; ++dif.y) {
            // find Chunk in chunk_it->second
            chunk_it = chunks.find(chunk_pos + dif);
            // tests, if chunk already exists in map
            if (chunk_it != chunks.end()) {
                Chunk* tmp = &chunk_it->second;
                if (!tmp->is_active()) {
                    server->wake(tmp);
                }
            } else {
                server->load(chunk_pos + dif);
            }
        }
    }
}
