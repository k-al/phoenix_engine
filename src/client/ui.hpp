// header guard
#ifndef UI_HEADER
#define UI_HEADER

#include <SFML/Graphics.hpp>

#include "../ivec.h"


class UI_element {
    
    iVec2 pos;
    iVec2 size;
    
    sf::Sprite sprite;
    
    std::function<void()> left_click; // what should executed if element is left clicked
    std::function<void()> right_click; // what should executed if element is right clicked
}

class UI {
public:
    Interaction input;
    
    std::vector<UI_element> elements;
    Controllable* follow = null_ptr;
    
    
    
}

#endif // header guard
