#include <vector>

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
    
    // get the vk_extensions needed by glfw
    std::vector<std::string> get_vk_extensions ();
    
    bool init ();
    
    static void glfw_setup ();
    
    static void glfw_cleanup ();
    
}
