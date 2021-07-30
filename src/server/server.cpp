
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


// END class Server

