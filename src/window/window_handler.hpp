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
    
    VkInstance instance;
    
    std::vector<std::string> validation_layers;
    
    uint32_t WIDTH = 800;
    uint32_t HEIGHT = 600;
    
    bool is_open = false;
    
    
    WindowHandler ();
    
    ~WindowHandler ();
    
    bool ini ();
    
    void main_loop ();
    
    // get the vk_extensions needed by glfw
    std::vector<std::string> glfw_get_vk_extensions ();
    
    void glfw_ini ();
    
    void glfw_cleanup ();
    
    void instance_ini ();
    
    bool check_extension_support (std::vector<std::string>);
    
    bool check_layer_support ();
    
    void debug_messenger_ini ();
    
    void surface_ini ();
};

#endif
