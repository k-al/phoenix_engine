


#include "interaction.hpp"

bool KeyboardInteraction::get_button (Action bt) {
    return (this->is_mouse[std::static_cast<unsigned int>(bt)] && sf::Mouse::isButtonPressed(this->mouse_bindings[std::static_cast<unsigned int>(bt)]))
                || sf::Keyboard::isKeyPressed(this->key_bindings[std::static_cast<unsigned int>(bt)]);
}

std::vector<bool> KeyboardInteraction::get_actions () {
    return std::vector<bool>();
}

iVec2 KeyboardInteraction::get_move_dir () {
    
}
