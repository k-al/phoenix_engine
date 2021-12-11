// header guard
#ifndef VISIBLE_HEADER
#define VISIBLE_HEADER

#include "thing.hpp"

#include "../defs.hpp"

#include <SFML/Graphics.hpp>
#include <string>
#include <array>

class Visible : virtual public Thing {
    sf::Sprite sprite;
    
    // the path of the texture
    std::string& texture_name;
    
    // startpoints of the animations
    // must ended with the length of the whole spritesheet
    std::array<uint16>& animations;
    // current sprite texture
    uint16 animation_frame = 0;
    // end of current animation
    uint16 animation_end = 0;
    
    // the last 8 bit of the timestamp when the animation was last updated (for timelapsing)
    uint8 last updated = 0;
    // true if animation should loop (don't reset next_animation)
    bool loop;
    // number of next animation (index of this->animations)
    size_t next_animation = 0;
    
    // update and return the pointer to the Sprite for the next frame
    sf::Sprite* update_sprite (uint64);
    
    // set the actual frame on the sprite by moving the texure offset
    void set_animation_frame (uint16);
    
    // change the used texture
    //? change this to bool to indicate if the new texture existed?
    void update_texture (std::string&, std::array<uint16>&);
}

#endif
