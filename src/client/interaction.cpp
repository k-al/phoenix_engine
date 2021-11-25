


#include "interaction.hpp"

bool KeyboardInteraction::get_button (Action bt) {
    return (this->is_mouse[static_cast<unsigned int>(bt)] && sf::Mouse::isButtonPressed(this->mouse_bindings[static_cast<unsigned int>(bt)]))
                || sf::Keyboard::isKeyPressed(this->key_bindings[static_cast<unsigned int>(bt)]);
}

//# that is absolutly ugly
std::vector<bool> KeyboardInteraction::get_actions () {
    std::vector<bool> res(int(Action::count));
    
    for (unsigned int i = 0; i < res.size(); i++) {
        res[i] = get_button(static_cast<Action>(i));
    }
    
    return res;
}

iVec2 KeyboardInteraction::get_move_dir () {
    // cross must be sqrt(2) / 2 times the speed in one direction in both directions
    static const int64_t straight = 0x400;
    static const int64_t cross    = 0x2D4;
    
    
    int vertical   = static_cast<int>(this->get_button(Action::up)) - static_cast<int>(this->get_button(Action::down));
    int horizontal = static_cast<int>(this->get_button(Action::right)) - static_cast<int>(this->get_button(Action::left));
    
    // the multipications are just with -1 / 1 to get the right sign
    if (vertical != 0) {
        if (horizontal != 0) {
            return iVec2(cross * horizontal, cross * vertical);
        } else {
            return iVec2(0, straight * vertical);
        }
    } else {
        return iVec2(straight * horizontal, 0);
    }
}
