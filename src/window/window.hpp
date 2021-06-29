// Headder guard
#ifndef VUL_WINDOW_HEADER
#define VUL_WINDOW_HEADER


#include "window_handler.hpp"
#include "device.hpp"


class Window {
  public:
    WindowHandler window_handler;
    Device device;
    
    Window ();
    
    ~Window ();
    
    bool ini ();
    
    
};

#endif
