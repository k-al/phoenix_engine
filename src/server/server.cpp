
// #include <string> // eingebunden durch .hpp
// #include <chrono> // eingebunden durch .hpp

#include <iostream>

#include "server/chunk.hpp"
#include "multh_listworker.hpp"

#include "server.hpp"

// BEGIN class Server

// Plan map;


Server::Server () {}

Server::~Server () {
    std::cout << "Server stops...\n";
    
    
    
    //! stop loader here
    
    std::cout << "Server is stopped\n";
}

bool Server::ini (Server_ini ini) {
    if (this->is_ini) {
        std::cout << "Server reinitialisation...";
    }
    std::cout << "initialze Server...\n";
    
    multh::Listworker_ini<Chunk> lw_ini;
    lw_ini.thread_count = ini.thread_count;
    lw_ini.cycle_time = ini.cycletime;
    lw_ini.process_element = Chunk::tick;
    
    if (!this->worker.ini(lw_ini)) {
        std::cout << "Server initialisation fails due to initalisation of Listworker fails in Line " << __LINE__ << " of file " << __FILE__ << std::endl;
        return false;
    }
    
    this->is_ini = true;
    std::cout << "Server initialzed\n";
    return true;
}

bool Server::start () {
    std::cout << "Server starts...\n";
    
    if (!this->is_ini || this->is_run) {
        std::cerr << "Server requestet to start, but in false state:\n    initialized: " << this->is_ini << "\n    already running: " << this->is_run << std::endl;
        exit(800);
    }
    
    this->worker.start();
    
    std::cout << "Server is started\n";
    
    return true;
}

bool Server::load (iVec2 pos) {
    //! load from memory
    
    //! this is ugly as hell but crates a pair iVec2(pos) Chunk(pos)
    this->chunks.emplace(pos, pos);
    
    return this->chunks[pos].is_generated;
}

//? outsource complete function
void Server::generate (iVec2 pos) {
    //! give job to the world-generator
    this->chunks[pos].is_generated = true;
}

void Server::wake (iVec2 pos) {
    // find Chunk in chunk_it->second
    auto chunk_it = this->chunks.find(pos);
    // tests, if chunk realy exists
    if (chunk_it != this->chunks.end()) {
        this->wake(&chunk_it->second);
    }
}

void Server::sleep (iVec2 pos) {
    // find Chunk in chunk_it->second
    auto chunk_it = this->chunks.find(pos);
    // tests, if chunk realy exists
    if (chunk_it != this->chunks.end()) {
        this->sleep(&chunk_it->second);
    }
}


// END class Server

