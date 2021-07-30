
#ifndef LOADER_HEADER
#define LOADER_HEADER


#include <string>

#include "ivec.hpp"
#include "thing.hpp"
#include "multh_listworker.hpp"

struct Thing;
struct Chunk;
struct Plan;

struct Loader_ini {
    std::string saving_loc;
    uint64_t thread_count = 2;
    Plan* map;
    multh::Listworker<Chunk>* server_worker;
};

class Loader {
  public:
    
    bool is_ini = false;
    bool is_run = false;
    
    std::string saving_loc;
    Plan* map;
    
    multh::Listworker<Thing> worker;
    multh::Listworker<Chunk>* server_worker;
    
    
    Loader ();
    
    ~Loader ();
    
    bool ini (Loader_ini);
    
    void start ();
    
    static void check_loaded_chunks (Thing*, uint64_t);
    
    bool load (iVec2 pos);
    
    void generate (iVec2 pos);
    
    void wake (iVec2 pos);
    
    inline void wake (Chunk* ptr) {
        this->server_worker->add(ptr);
    }
    
    void sleep (iVec2 pos);
    
    inline void sleep (Chunk* ptr) {
        this->server_worker->del(ptr);
    }
};

#endif // LOADER_HEADER
