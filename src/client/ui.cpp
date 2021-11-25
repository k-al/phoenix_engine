

#include "ui.hpp"

#include "interaction.hpp"

void UI::change_follow (Controllable* obj) {
    this->follow = obj;
}

bool UI::process_input (std::vector<bool> inputs, iVec2 move_dir, iVec2 cursor_pos) {
    
    
    
    if (this->follow)
        this->follow->action(this->follow, inputs, iVec2 move_dir, iVec2 cursor_pos);
}
