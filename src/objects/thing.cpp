
#include "server/chunk.hpp"
#include "server/server.hpp"
#include "ivec.hpp"

#include "thing.hpp"

Thing::Thing () {}

Thing::Thing (Server* server) {this->server = server;}

//# dont use full coll_check???
bool Thing::move (const iVec2& direction) {
    bool res = true;
    std::vector<Thing*> collisions = this->coll_check(direction);
    
    // set back object if it would collide with something, that is solide to it
    //# test if this has moved?
    if (this->is_solide(nullptr)) { // check if can collide with something
        iVec2 temp_upper = this->upper_bound() + direction;
        iVec2 temp_lower = this->lower_bound() + direction;
        iVec2 temp_dir = direction;
        
        for (const Thing* coll : collisions) {
            if (this->is_solide(coll) && coll->is_solide(this)) {
                res = false; // collided with something, so aimed position is probably not reachable
                int64_t xdiff = 0;
                int64_t ydiff = 0;
                // the real work is done after the '&&'
                if (direction.x < 0 && (xdiff = coll->upper_bound().x - temp_lower.x) < 0) {
                    xdiff = 0;
                } else if (direction.x > 0 && (xdiff = coll->lower_bound().x - temp_upper.x) > 0) {
                    xdiff = 0;
                }
                if (direction.y < 0 && (ydiff = coll->upper_bound().y - temp_lower.y) < 0) {
                    ydiff = 0;
                } else if (direction.y > 0 && (ydiff = coll->lower_bound().y - temp_upper.y) > 0) {
                    ydiff = 0;
                }
                
                //? push obj here
                
                // correct the new position so hopefully no collision occurs
                if (abs(xdiff) < abs(ydiff)) {
                    temp_lower.x += xdiff;
                    temp_upper.x += xdiff;
                    temp_dir.x += xdiff;
                } else {
                    temp_lower.y += ydiff;
                    temp_upper.y += ydiff;
                    temp_dir.y += ydiff;
                }
            }
        }
        
        this->position += temp_dir;
        
        static const int64_t overflow_bitmask = ~((Chunk::coo_max * 2) + 1); //? safe, that it isnt '- 1'
        
//         if () {
//             
//         }
        
        if (this->position.x > Chunk::coo_max) {
            
        } else if (this->position.x < Chunk::coo_min) {
            
        }
        
        // relocation here with temp_dir
    }
    
    return res;
}

bool Thing::chunk_change (const iVec2& new_chunk_pos) {
    // find Chunk in chunk_it->second
    auto map_it = this->server->chunks.find(new_chunk_pos);
    // tests, if chunk realy exists
    if (map_it == this->server->chunks.end()) {
        return false;
    } else {
        this->chunk_change(&map_it->second); //? test what version this calls in inheritance
        return true;
    }
}

bool Thing::chunk_change (Chunk* new_chunk) {
    
    // change the owner chunk
    new_chunk->add(this);
    this->chunk = new_chunk;
    this->chunk->remove(this);
    
    // if object loads surounding let it do it
    if (this->load_range != 0) {
        this->load_suround();
    }
    
    return true;
}

void Thing::load_suround () const {
    auto chunk_it = this->server->chunks.end();
    iVec2 dif(0, 0);
    
    for (dif.x = -this->load_range; dif.x <= this->load_range; ++dif.x) {
        for (dif.y = this->load_range; dif.y <= this->load_range; ++dif.y) {
            // find Chunk in chunk_it->second
            chunk_it = this->server->chunks.find(this->chunk->position + dif);
            // tests, if chunk already exists in map
            if (chunk_it != this->server->chunks.end()) {
                if (!chunk_it->second.is_active()) {
                    this->server->wake(&chunk_it->second);
                }
                
            } else {
                this->server->load(this->chunk->position + dif);
            }
        }
    }
}

std::vector<Thing*> Thing::coll_check (iVec2 direction) const {
    std::vector<Thing*> res;
    
    iVec2 temp_upper = this->upper_bound() + direction;
    iVec2 temp_lower = this->lower_bound() + direction;
    
    // collision detection
    //! chunk borders are dangerous
    for (Thing* other : this->chunk->objects) {
        //# store other.X_bounds temporaly, so it doesnt compute twice
        if (temp_upper.x > other->lower_bound().x
            && temp_lower.x < other->upper_bound().x
            && temp_upper.y > other->lower_bound().y
            && temp_lower.y < other->upper_bound().y
        ) {
            res.push_back(other);
        }
    }
    return res;
}

std::vector<Thing*> Thing::coll_check (const iVec2& upper_bound, const iVec2& lower_bound, const std::vector<Thing*>& others) const {
    std::vector<Thing*> res;
    
    for (Thing* other : others) {
        //# store other.X_bounds temporaly, so it doesnt compute twice
        if (upper_bound.x > other->lower_bound().x
            && lower_bound.x < other->upper_bound().x
            && upper_bound.y > other->lower_bound().y
            && lower_bound.y < other->upper_bound().y
        ) {
            res.push_back(other);
        }
    }
    return res;
}

