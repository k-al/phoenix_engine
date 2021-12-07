#include <iostream>
#include "../client/interaction.hpp"

#include "player.hpp"

Player::Player () {
    this->action = [this](Controllable* obj, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos)->void{
        
        move_dir *= this->speed;
        move_dir >>= 10;
        
        this->move(move_dir);
    };
    
    this->tick_hook = [this](uint64_t time)->void{
        return;
    };
}

Player::~Player () {}

void Player::tick (uint64_t time) {
    return;
}


void Player::default_action (Controllable* obj, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos) {
    
}
