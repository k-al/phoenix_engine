#ifndef CHUNK_HEADER
#define CHUNK_HEADER

#include <vector>
#include <atomic>
#include <set>

#include "ivec.hpp"

struct Thing;

struct Chunk {
    // use 32 bit for coordination in chunks, but reserve 6 of them for overflow detection
    static const int64_t size = 0x08000000; // should be 0b1[0]{26}
    
    // the real range of coordinates values
    static const int64_t coo_max = (size / 2) - 1; // should be 0x03FFFFFF
    static const int64_t coo_min = -(size / 2); // should be 0xFBFFFFFF
    
    // object-pointers
    std::vector<Thing*> objects;
    
    iVec2 position;
    bool is_generated;
    bool is_loaded;
    bool is_visible;
    
    std::set<Thing*> loaded_by;
    
    //! for locking in multithread
    // std::shared_mutex lock;
    
    // informations used and managed by loader
    std::atomic<uint64_t> multh_del_it[1] = {0xFFFFFFFFFFFFFFFF}; // iterator to chunk_list for fast deleting
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
