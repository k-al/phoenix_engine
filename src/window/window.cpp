


// #include "window_handler.hpp" // included by .hpp
// #include "device.hpp" // included by .hpp

#include "window.hpp"

Window::Window () {}

Window::~Window () {}

bool Window::ini () {
    this->window_handler.ini();
    
    return true;
}
