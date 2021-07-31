#ifndef SERVER_HEADER
#define SERVER_HEADER

#include <string>
#include <chrono>

#include "ivec.hpp"
#include "server/chunk.hpp"

#include "multh_listworker.hpp"

struct Server_ini {
    uint64_t thread_count = 2;
    std::chrono::duration<int64_t, std::milli> cycletime = std::chrono::milliseconds(20);
};


class Server {
  public:
    // maps
    struct MapKeyHasher{
        int64_t operator()(const iVec2& k) const {
            return k.x << 32 | k.y;
        }
    };
    
    std::unordered_map<iVec2, Chunk, MapKeyHasher> chunks;
    
    // server controlling
    bool is_ini = false;
    bool is_run = false;
    
    multh::Listworker<Chunk> worker;
    
    Server (/*saving location*/);
    
    ~Server ();
    
    
    // server controlling
    bool ini (Server_ini ini);
    
    bool start ();
    
    
    // chunk loading/unloading
    bool load (iVec2 pos);
    
    void generate (iVec2 pos);
    
    void wake (iVec2 pos);
    
    inline void wake (Chunk* ptr) {
        this->worker.add(ptr);
    }
    
    void sleep (iVec2 pos);
    
    inline void sleep (Chunk* ptr) {
        this->worker.del(ptr);
    }
    
    
    // handeling players
    void join (std::string huuid);
};

#endif // SERVER_HEADER
