


#include "interaction.hpp"


#include <iostream> // just for debugging

Interaction::~Interaction () {}

KeyboardInteraction::KeyboardInteraction () {}

KeyboardInteraction::~KeyboardInteraction () {}

bool KeyboardInteraction::get_button (Action bt) {
    return this->is_mouse[static_cast<unsigned int>(bt)]
                ? sf::Mouse::isButtonPressed(this->mouse_bindings[static_cast<unsigned int>(bt)])
                : sf::Keyboard::isKeyPressed(this->key_bindings[static_cast<unsigned int>(bt)]);
}

//# that is absolutly ugly
std::vector<bool> KeyboardInteraction::get_actions () {
    std::vector<bool> res(int(Action::count));
    
    for (unsigned int i = 0; i < res.size(); i++) {
        res[i] = get_button(static_cast<Action>(i));
    }
    
    return res;
}

void KeyboardInteraction::set_default_bindings () {
    this->key_bindings[static_cast<unsigned int>(Action::up)]            = sf::Keyboard::W;
    this->key_bindings[static_cast<unsigned int>(Action::down)]          = sf::Keyboard::S;
    this->key_bindings[static_cast<unsigned int>(Action::left)]          = sf::Keyboard::A;
    this->key_bindings[static_cast<unsigned int>(Action::right)]         = sf::Keyboard::D;
    this->key_bindings[static_cast<unsigned int>(Action::sprint)]        = sf::Keyboard::LShift;
    this->key_bindings[static_cast<unsigned int>(Action::crouch)]        = sf::Keyboard::LControl;
    this->key_bindings[static_cast<unsigned int>(Action::dash)]          = sf::Keyboard::Space;
    this->key_bindings[static_cast<unsigned int>(Action::inventar)]      = sf::Keyboard::F;
    this->key_bindings[static_cast<unsigned int>(Action::use)]           = sf::Keyboard::Y;
    this->key_bindings[static_cast<unsigned int>(Action::reload)]        = sf::Keyboard::R;
    this->key_bindings[static_cast<unsigned int>(Action::hotbar_1)]      = sf::Keyboard::X;
    this->key_bindings[static_cast<unsigned int>(Action::hotbar_2)]      = sf::Keyboard::C;
    this->key_bindings[static_cast<unsigned int>(Action::hotbar_3)]      = sf::Keyboard::V;
    this->key_bindings[static_cast<unsigned int>(Action::item_1)]        = sf::Keyboard::Num1;
    this->key_bindings[static_cast<unsigned int>(Action::item_2)]        = sf::Keyboard::Num2;
    this->key_bindings[static_cast<unsigned int>(Action::item_3)]        = sf::Keyboard::Num3;
    this->key_bindings[static_cast<unsigned int>(Action::item_4)]        = sf::Keyboard::Num4;
    this->key_bindings[static_cast<unsigned int>(Action::item_5)]        = sf::Keyboard::Num5;
    this->mouse_bindings[static_cast<unsigned int>(Action::prim_attack)] = sf::Mouse::Left;
    this->is_mouse[static_cast<unsigned int>(Action::prim_attack)]       = true;
    this->mouse_bindings[static_cast<unsigned int>(Action::sec_attack)]  = sf::Mouse::Right;
    this->is_mouse[static_cast<unsigned int>(Action::sec_attack)]        = true;
    this->mouse_bindings[static_cast<unsigned int>(Action::tert_attack)] = sf::Mouse::Middle;
    this->is_mouse[static_cast<unsigned int>(Action::tert_attack)]       = true;
}

iVec2 KeyboardInteraction::get_move_dir () {
    // cross must be sqrt(2) / 2 times the speed in one direction in both directions
    static const int64_t straight = 0x400;
    static const int64_t cross    = 0x2D4;
    
    
    int vertical   = static_cast<int>(this->get_button(Action::up)) - static_cast<int>(this->get_button(Action::down));
    int horizontal = static_cast<int>(this->get_button(Action::right)) - static_cast<int>(this->get_button(Action::left));
    
    std::cout << std::dec << "got (" << horizontal << "|" << vertical << ") with";
    std::cout << " up: " << this->get_button(Action::up);
    std::cout << " down: " << this->get_button(Action::down);
    std::cout << " left: " << this->get_button(Action::left);
    std::cout << " right: " << this->get_button(Action::right) << std::endl;
    
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

iVec2 KeyboardInteraction::get_cursor_pos () {
    return sf::Mouse::getPosition();
}
