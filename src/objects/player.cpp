#include <iostream>
#include "../client/interaction.hpp"

#include "player.hpp"

Player::Player () {
    this->action = &Player::default_action;
}

void Player::default_action (Thing* obj, std::vector<bool> actions, iVec2 move_dir, iVec2 cursor_pos) {
    Player* that = static_cast<Player*>(obj);
    
    move_dir *= that->speed;
    move_dir >>= 10;
    
    that->move(move_dir);
}
