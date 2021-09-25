
#include "ivec.hpp"
#include "objects/thing.hpp"

class Visible : public Thing {
  public:
    sf::Sprite sprite;
    
    //# These are static informations for a type of objects. This not have to stored multiple times.
    std::string texture;
    std::vector<uint64_t> animations; // start and endpoits of the frames for all animations
    
    uint64_t last_tick; // Last game_tick that this object has been updatet. For timelapsing.
    
    uint64_t current_frame; // current frame of animation
    uint64_t end_frame; // end point of current animation
    
    sf::Sprite* update (uint64_t game_tick, iVec2 active_chunk, iVec2 center_position);
    bool load_texture ();
    void unload_texture ();
    
    
};
