#ifndef PLAN_HEADER
#define PLAN_HEADER

#include <unordered_map>

#include "ivec.hpp"
#include "chunk.hpp"


class Plan {
  public:
    
    struct KeyHasher{
        int64_t operator()(const iVec2& k) const;
    };
    
    std::unordered_map<iVec2, Chunk, KeyHasher> chunks;
    
    Plan ();
    
};

#endif // PLAN_HEADER
