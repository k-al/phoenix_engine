// #include <unordered_map> // eingebunden durch .hpp

// #include "ivec.hpp" // eingebunden durch .hpp
// #include "chunk.hpp" // eingebunden durch .hpp

#include "plan.hpp"


// BEGIN class Plan


 int64_t Plan::KeyHasher::operator()(const iVec2& k) const {
     return k.x << 32 | k.y;
 }

Plan::Plan () {}

// END class Plan
