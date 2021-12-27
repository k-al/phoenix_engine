#ifndef CHUNK_HEADER
#define CHUNK_HEADER

#include <vector>
#include <atomic>
#include <set>

#include "ivec.hpp"

struct Thing;
struct Server;

struct Chunk {
    // use 32 bit for coordination in chunks, but reserve 6 of them for overflow detection
    static constexpr uint32 size = 0x03FFFFFF; // should be 0b1[0]{26}
    
    // the real range of coordinates values
    static constexpr uint32 coo_max = Chunk::size; // should be 0x03FFFFFF
    static constexpr uint32 coo_min = 0; // should be 0x0
    
    // object-pointers
    std::vector<Thing*> objects;
    
    Server* server;
    iVec2 position;
    bool is_generated;
    bool is_loaded;
    bool is_visible;
    
    std::set<Thing*> loaded_by;
    
    //! for locking in multithread
    // std::shared_mutex lock;
    
    // informations used and managed by loader
    std::atomic<uint64_t> multh_del_it[1] = {uint64_MAX}; // iterator to chunk_list for fast deleting
    std::atomic<bool> multh_added[1] = {false}; // flag if in add_list
    
    Chunk ();
    
    Chunk (iVec2 pos);
    
    ~Chunk ();
    
    void remove (Thing*);
    
    void add (Thing*);
    
    inline bool is_active () {
        return this->multh_added[0].load();
    }
    
    static void tick (Chunk*, uint64_t);
};

#endif //CHUNK_HEADER
