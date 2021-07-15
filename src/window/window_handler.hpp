// Headder guard
#ifndef VUL_HANDLER_HEADER
#define VUL_HANDLER_HEADER

#include <vector>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


struct WindowHandler_ini {
    std::vector<std::string> validation_layers;
    
    WindowHandler_ini (std::vector<std::string> validation_layers) {
        this->validation_layers = validation_layers;
    }
};

class WindowHandler {
  public:
    GLFWwindow* main_window;
    
    const char** glfw_extensions = nullptr;
    uint32_t glfw_extensions_count = 0;
    
    VkInstance instance;
    
    std::vector<std::string> validation_layers;
    
    uint32_t WIDTH = 800;
    uint32_t HEIGHT = 600;
    
    bool is_ini = false;
    
    
    WindowHandler ();
    
    ~WindowHandler ();
    
    bool ini (WindowHandler_ini);
    
    void main_loop ();
    
    ///////////////////////////
    // GLFW-window
    ///////////////////////////
    
    void glfw_ini ();
    
    void glfw_cleanup ();
    
    // get the vk_extensions needed by glfw
    std::vector<std::string> glfw_get_vk_extensions (bool as_vec);
    
    
    ///////////////////////////
    // VkInstance
    ///////////////////////////
    
    void instance_ini ();
    
    bool check_extension_support (std::vector<std::string>);
    
    bool check_layer_support ();
    
    
    ///////////////////////////
    // VkDebugMessenger
    ///////////////////////////
    //! needs implementation
    
    void debug_messenger_ini ();
    
    ///////////////////////////
    // VkSurface
    ///////////////////////////
    //! needs implementation
    
    void surface_ini ();
};

#endif
