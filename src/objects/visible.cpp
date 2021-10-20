

#include "visible.hpp"

bool Visible::chunk_change (Chunk* new_chunk) {
    if (this->chunk->is_visible) {
        if (!new_chunk->is_visible) {
            // take out object from renderer
        }
    } else {
        if (new_chunk->is_visible) {
            // introduce object to renderer
        }
    }
}
