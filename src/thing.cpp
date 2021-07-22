
#include "chunk.hpp"
#include "ivec.hpp"
#include "plan.hpp"
#include "loader.hpp"

#include "thing.hpp"

bool Thing::move (const iVec2& direction) {
    iVec2 temp_upper = this->upper_bounds() + direction;
    iVec2 temp_lower = this->lower_bounds() + direction;
    
    
    // collision detection
    //! chunk borders are dangerous
    for (const Thing* other : this->chunk) {
        //# store other.X_bounds temporaly, so it doesnt compute twice
        if (temp_upper.x > other.lower_bounds.x
            && temp_lower.x < other.upper_bounds.x
            && temp_upper.y > other.lower_bounds.y
            && temp_lower.y < other.upper_bounds.y
        ) {
            // hit
        }
    }
}

bool Thing::chunk_change (const iVec2& new_chunk_pos) {
    // find Chunk in chunk_it->second
    auto map_it = this->map->chunks.find(new_chunk_pos);
    // tests, if chunk realy exists
    if (map_it == this->map->chunks.end()) {
        return false;
    } else {
        this->chunk_change(&map_it->second);
        return true;
    }
}

bool Thing::chunk_change (Chunk* new_chunk) {
    
    new_chunk->add(this);
    this->chunk->remove(this);
    
    this->chunk = new_chunk;
    
    if (this->load_range != 0) {
        this->load_suround();
    }
    
    return true;
}

void Thing::load_suround () {
    auto chunk_it = this->map->chunks.end();
    iVec2 dif(0, 0);
    
    for (dif.x = -this->load_range; dif.x <= this->load_range; ++dif.x) {
        for (dif.y = this->load_range; dif.y <= this->load_range; ++dif.y) {
            // find Chunk in chunk_it->second
            chunk_it = this->map->chunks.find(this->chunk->position + dif);
            // tests, if chunk already exists in map
            if (chunk_it != this->map->chunks.end()) {
                if (!chunk_it->second.is_active()) {
                    this->loader->wake(&chunk_it->second);
                }
                
            } else {
                this->loader->load(this->chunk->position + dif);
            }
        }
    }
}
