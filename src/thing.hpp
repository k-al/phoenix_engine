// header guard
#ifndef THING_HEADER
#define THING_HEADER

#include <set>
#include <atomic>
#include <typeinfo>

#include "ivec.hpp"


struct Chunk;
struct Plan;
struct Loader;

//# use static polymorphism instead of virtual functions
struct Thing {
  public:
      
    //! get the loader from somewhere
    Loader* loader;
    
    // localisation:
    Plan* map;
    Chunk* chunk;
    iVec2 position;
    
    bool solide;
    
    bool is_ooi;
    std::string ooiid;
    
    uint16_t load_range; // chunk radius wich is loaded by this object (0 for none)
    
    virtual void tick (uint64_t tick_time) = 0;
    virtual uint16_t push (Thing* pusher, uint16_t dist, uint16_t dir) = 0;;
    
    
    bool chunk_change (iVec2 new_chunk);
    void chunk_change (Chunk* new_chunk);
    
    void load_suround ();
    
    
    // informations used and managed by loader
    std::atomic<uint64_t> multh_del_it[1] = {0xFFFFFFFFFFFFFFFF}; // iterator to chunk_list for fast deleting
    std::atomic<bool> multh_added[1] = {false}; // flag if in add_list
    
};

#endif // THING_HEADER
