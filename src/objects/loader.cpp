
#include "loader.hpp"

#include "../server/server.hpp"

void Loader::load (int32 range = -1) {
    if (range != -1)
        this->range = range;
    
    // initalise the new active_chunks with the maximum capacity to reduce memory allocations
    std::vector<Chunk*> active_chunks();
    active_chunks.reserve((this->range * 2 + 1) * (this->range * 2 + 1));
    
    const Server* server = this->chunk->server;
    const std::unordered_map<iVec2, Chunk, Server::MapKeyHasher>& chunks = server->chunks;
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
                active_chunks(tmp);
            } else {
                server->load(chunk_pos + dif);
            }
        }
    }
    
    this->active_chunks.swap(active_chunks);
}
