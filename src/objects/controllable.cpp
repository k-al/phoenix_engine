
#include "controllable.hpp"

void Controllable::action (std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos) {
    return;
}


void Controllable::get_active_chunks (std::vector<Chunk*>& buffer, uint32 range) {
    
    // initalise the new active_chunks with the maximum capacity to reduce memory allocations
    std::vector<Chunk*> active_chunks();
    active_chunks.reserve((this->range * 2 + 1) * (this->range * 2 + 1));
    
    const Server* server = this->chunk->server;
    const auto& chunks = server->chunks; // auto should compile to std::unordered_map<iVec2, Chunk, Server::MapKeyHasher>
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
                active_chunks.push_back(tmp);
            } else {
                server->load(chunk_pos + dif);
            }
        }
    }
    
    buffer.swap(active_chunks);
}

bool Controllable::get_active_chunks (std::vector<Chunk*>& buffer) {
    if (this->active_chunks.empty()) {
        return false;
    } else {
        buffer.swap(this->active_chunks);
        return true;
    }
}


// if the standart range should be loaded, update active chunks
virtual void Controllable::load (uint32 range) {
    if (this->range == range)
        return this->get_active_chunks(this->active_chunks, range);
    
    return this->Loader::load(range);
}
