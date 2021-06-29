


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Device {
    VkPhysicalDevice physical = VK_NULL_HANDLE; // the used graphic card
    VkDevice logical; // the logical instance of the used graphic card
    
    bool is_ini = false;
    
    Device ();
    
    ~Device ();
    
    bool ini ();
    
    void pickPhysicalDevice();
}
