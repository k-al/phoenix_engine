#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <string>
#include <chrono>

#include "plan.hpp"
#include "loader.hpp"

#include "multh_listworker.hpp"

struct Chunk;

struct Server_ini {
    uint64_t thread_count = 2;
    std::chrono::duration<int64_t, std::milli> cycletime = std::chrono::milliseconds(20);
};


class Server {
  public:
    Plan map; // the Plan as Map
    Loader loader; // manages wich chunk shuold be loaded/unloaded
    
    bool is_ini = false;
    bool is_run = false;
    
    multh::Listworker<Chunk> worker;
    
    Server (/*saving location*/);
    
    ~Server ();
    
    bool ini (Server_ini ini);
    
    bool start ();
    
    void join (std::string huuid);
};

#endif // SERVER_HEADER
