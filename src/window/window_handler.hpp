// Headder guard
#ifndef VUL_HANDLER_HEADER
#define VUL_HANDLER_HEADER

#include <vector>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class WindowHandler {
  public:
    GLFWwindow* main_window;
    
    uint32_t WIDTH = 800;
    uint32_t HEIGHT = 600;
    
    bool is_open = false;
    
    
    WindowHandler ();
    
    ~WindowHandler ();
    
    bool ini ();
    
    void main_loop ();
    
    // get the vk_extensions needed by glfw
    std::vector<std::string> get_vk_extensions ();
    
    static void glfw_setup ();
    
    static void glfw_cleanup ();
    
};

#endif
