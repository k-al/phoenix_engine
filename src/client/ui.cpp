
#include "interaction.hpp"
#include "../objects/controllable.hpp"

#include "ui.hpp"

void UI::change_follow (Controllable* obj) {
    this->follow = obj;
}

bool UI::process_input (std::vector<bool> inputs, iVec2 move_dir, iVec2 cursor_pos) {
    
    // fetch all inputs that affect only the UI
    
    // hand the other inputs to the controlled object
    
    if (this->follow)
        this->follow->action(inputs, move_dir, cursor_pos);
    
    return true;
}
