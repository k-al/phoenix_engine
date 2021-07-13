
#include <chrono>
#include <iostream>
// #include <string> // eingebunden durch .hpp

#include "plan.hpp"
#include "chunk.hpp"
// #include "thing.hpp" // eingebunden durch .hpp
// #include "ivec.hpp" // eingebunden durch .hpp
// #include "multh_listworker.hpp" // eingebunden durch .hpp

#include "loader.hpp"

Loader::Loader () {}

Loader::~Loader () {
    std::cout << "Loader stops...\n";
    
    if (this->is_run) {
        this->worker.halt();
    }
    
    std::cout << "Loader is stopped\n";
}

// returns if it can find and modify saving_loc
bool Loader::ini(Loader_ini ini) {
    std::cout << "initialze Loader...\n";
    
    if (ini.map == nullptr) {
        std::cout << "Loader initialisation fails due to null pointer for Map pointer in Line " << __LINE__ << " of file " << __FILE__ << std::endl;
        return false;
    }
    if (ini.server_worker == nullptr) {
        std::cout << "Loader initialisation fails due to null pointer for Server-worker pointer in Line " << __LINE__ << " of file " << __FILE__ << std::endl;
        return false;
    }
    
    this->saving_loc = ini.saving_loc;
    this->map = ini.map;
    this->server_worker = ini.server_worker;
    
    multh::Listworker_ini<Thing> l_ini;
    l_ini.thread_count = ini.thread_count;
    l_ini.cycle_time = std::chrono::milliseconds(500);
    l_ini.del_it_pos = 0;
    l_ini.process_element = Loader::check_loaded_chunks;
    
    if (!this->worker.ini(l_ini)) {
        std::cout << "Loader initialisation fails due to initalisation of Listworker fails in Line " << __LINE__ << " of file " << __FILE__ << std::endl;
        return false;
    }
    std::cout << "Loaderworker initialzed\n";
    
    this->is_ini = true;
    std::cout << "Loader initialzed\n";
    return true;
}

void Loader::start () {
    std::cout << "Loader starts...\n";
    
    if (!this->is_ini || this->is_run) {
        std::cerr << "Loader requestet to start, but in false state:\n    initialized: " << this->is_ini << "\n    already running: " << this->is_run << std::endl;
        exit(800);
    }
    
//     this->worker.start();
    
    std::cout << "Loader is started\n";
}

void Loader::check_loaded_chunks(Thing* t, uint64_t cycle) {
    return;
}

bool Loader::load (iVec2 pos) {
    //! load from memory
    
    //! this is ugly as hell but crates a pair iVec2(pos) Chunk(pos)
    this->map->chunks.emplace(pos, pos);
    
    return map->chunks[pos].is_generated;
}

//? outsource complete function
void Loader::generate (iVec2 pos) {
    //! give job to the world-generator
    this->map->chunks[pos].is_generated = true;
}

void Loader::wake (iVec2 pos) {
    // find Chunk in chunk_it->second
    auto chunk_it = this->map->chunks.find(pos);
    // tests, if chunk realy exists
    if (chunk_it != this->map->chunks.end()) {
        this->wake(&chunk_it->second);
    }
}

void Loader::sleep (iVec2 pos) {
    // find Chunk in chunk_it->second
    auto chunk_it = this->map->chunks.find(pos);
    // tests, if chunk realy exists
    if (chunk_it != this->map->chunks.end()) {
        this->sleep(&chunk_it->second);
    }
}

// END class Loader

