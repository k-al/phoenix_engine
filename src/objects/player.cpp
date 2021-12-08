#include "../client/interaction.hpp"

#include "player.hpp"

#include <iostream> // just for debug


Player::Player () {
    this->speed = 0x11;
    
    this->action = [this](std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos)->void{
        
        move_dir *= this->speed;
        move_dir >>= 10;
        
        this->move(move_dir);
        
        std::cout << "moved in (" << move_dir.x << "|" << move_dir.y << ")\n";
    };
    
    this->tick_hook = [this](uint64_t time)->void{
        return;
    };
    
    this->is_solide = [](const Thing*)->bool{return true;}; // always collide with everything
}

Player::~Player () {}

void Player::tick (uint64_t time) {
    return;
}


void Player::default_action (Controllable* obj, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos) {
    
}
