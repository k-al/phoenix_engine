// Headder guard
#ifndef VUL_WINDOW_HEADER
#define VUL_WINDOW_HEADER

#include <vector>
#include <string>

#include "window_handler.hpp"
#include "device.hpp"


class Window {
  public:
    WindowHandler window_handler;
    Device device;
    
    #ifdef NDEBUG
    const bool enable_validation = false;
    #else
    const bool enable_validation = true;
    #endif
    
    std::vector<std::string> validation_layers;
    
    void set_validation_layers ();
    
    Window ();
    
    ~Window ();
    
    bool ini ();
    
    void create_vulkan_instance ();
    
    
};

#endif
