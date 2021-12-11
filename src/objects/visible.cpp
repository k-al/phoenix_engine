
#include "../client/texture_manager.hpp"

#include "visible.hpp"


    // maximum save frames per sprite are max(uin16) - max(uin8) (aka: 65280) because of this implementation
            // no crashes if there are more, just possibly wierd behavior with timelapsing
sf::Sprite* Visible::update_sprite (uint64 time) {
    int16 time_diff = static_cast<int16>(static_cast<uint8>(time) - this->last_updated);
    this->last_updated = static_cast<uint8>(time);
    
    if (time_diff <= 0) // allready drawn this frame
        return &this->sprite;
    
    this->animation_frame += static_cast<uint16>(time_diff); // reinterpret because time_dif is > 0
    
    // if the animation is over set the current animation to the next animation
    if (this->animation_frame >= this->animation_end) {
        
        // if there are no animation queued leave it at the current frame
        //? switch to frame 0?
        if (this->next_animation == this->animations.size() - 1) {
            this->animation_frame = this->animation_end; // prevent overflowing
            return &this->sprite;
        }
        
        this->animation_frame = this->animations[this->next_animation];
         // + this->animation_frame - this->animation_end; // lapse into other animation ommited because of potential out-of-bounds exeption
        
        this->animation_end = this->animations[this->next_animation + 1];
        
        // if loop is disabled reset the this->next_animation
        if (!loop)
            this->next_animation = this->animations.size() - 1;
    }
    
    this->set_animation_frame(this->animation_frame);
    
    return &this->sprite;
}


void Visible::set_animation_frame (uint16 frame) {
    // dont go out of bounds
    if (frame > this->animations[this->animations.size() - 1])
        return;
    
    //! TODO: set the texture at the right position, so the current frame is displayed
    
}


void Visible::update_texture (std::string& new_tex_name, std::vector<uint16>& new_animations) {
    TextureManager::drop(this->texture_name);
    
    this->texture_name = new_tex_name;
    
    const sf::Texture& tex = TextureManager::get(this->texture_name);
    this->sprite.setTexture(tex);
    
    this->animations = new_animations;
    this->animation_frame = 0;
    this->animation_end = 0;
    this->next_animation = new_animations.size();
    this->set_animation_frame(0);
}

