#include <iostream>
// #include <vector> // eingebunden durch .hpp
// #include <atomic> // eingebunden durch .hpp
// #include <set> // eingebunden durch .hpp

#include "objects/thing.hpp"
// #include "ivec.hpp" // eingebunden durch .hpp

#include "chunk.hpp"

#include <thread> // just for debugging
#include <iostream>


Chunk::Chunk () {
    this->is_generated = false;
    std::cout << "default constructor of Chunk must not be used!\n";
    std::exit(701);
}

Chunk::Chunk (iVec2 pos) {
    std::cout << "got summond!\n";
    this->is_generated = false;
    this->position = pos;
}

Chunk::~Chunk () {
//     std::cout << "Chunk (" << this->position.x << "|" << this->position.y << ") is deleted.\n";
}

void Chunk::remove (Thing* del_ob) {
    //! lock chunk for modification
    
    //# store the iterator (somehow?!?) in the Object for fast deleting
    for (auto it = objects.begin(); it != objects.end(); it++) {
        if (*it == del_ob) {
            *it = objects.back();
            objects.pop_back();
        }
    }
    //! unlock chunk
}

void Chunk::add (Thing* add_ob) {
    //! lock chunk for modification
    objects.push_back(add_ob);
    //! unlock chunk
}

void Chunk::tick (Chunk* ch, uint64_t cycle_nr) {
    std::cout << "Server Tick Thread " << std::this_thread::get_id() << " gives tick\n";
    for (auto it = ch->objects.begin(); it != ch->objects.end(); ++it) {
        (*it)->tick(0);
    }
}

// END struct Chunk
