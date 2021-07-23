// header guard
#ifndef THING_HEADER
#define THING_HEADER

#include <set>
#include <atomic>
#include <typeinfo>
#include <vector>

#include "ivec.hpp"


struct Chunk;
struct Plan;
struct Loader;


class Thing {
  public:
    
    Thing ();
    
    virtual ~Thing ();
    
    //! get the loader from somewhere
    Loader* loader;
    
    // localisation:
    Plan* map;
    Chunk* chunk;
    iVec2 position;
    
    uint16_t load_range; // chunk radius wich is loaded by this object (0 for none)
    
    iVec2 collbox_size;
    inline iVec2 upper_bound () const { return this->position + (this->collbox_size / 2); } //? do i need these?
    inline iVec2 lower_bound () const { return this->position - (this->collbox_size / 2); }
    
    // properties
    std::function<bool(const Thing*)> is_solide;
    
    bool is_ooi;
    std::string ooiid;
    
    // hook functions
    std::function<void(uint64_t tick_time)> tick_hook;
    std::function<uint16_t(Thing* pusher, uint16_t dist, uint16_t dir)> push_hook;
    std::function<void(Thing* collider)> collide_hook;
    
    // basic interaction functions
    bool move (const iVec2& direction); // returns true if full length has been moved (nothing in the way)
    bool teleport (iVec2 position, iVec2 chunk, Plan* map, bool force); // return true if destination is free (nothing in the way)
    
    // background functions
    bool chunk_change (const iVec2& new_chunk); // returns false if chunk not found
    bool chunk_change (Chunk* new_chunk); // returns always true (chunk already found)
    void load_suround () const;
    std::vector<Thing*> coll_check (iVec2 direction = iVec2()) const;
    
    
    // informations used and managed by loader
    std::atomic<uint64_t> multh_del_it[1] = {0xFFFFFFFFFFFFFFFF}; // iterator to chunk_list for fast deleting
    std::atomic<bool> multh_added[1] = {false}; // flag if in add_list
    
};

#endif // THING_HEADER
