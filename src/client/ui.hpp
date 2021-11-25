// header guard
#ifndef UI_HEADER
#define UI_HEADER

#include <SFML/Graphics.hpp>

#include "../ivec.hpp"

class Controllable;

class UI_element {
    
    iVec2 pos;
    iVec2 size;
    
    sf::Sprite sprite;
    
    std::function<void()> left_click; // what should executed if element is left clicked
    std::function<void()> right_click; // what should executed if element is right clicked
};

class UI {
public:
    std::vector<UI_element> elements;
    Controllable* follow = nullptr;
    
    void change_follow (Controllable*);
    
    bool process_input (std::vector<bool> inputs, iVec2 move_dir, iVec2 cursor_pos);
    
};

#endif // header guard
