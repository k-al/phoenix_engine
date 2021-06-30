// Headder guard
#ifndef VUL_DEVICE_HEADER
#define VUL_DEVICE_HEADER

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


struct Device_ini {
    VkInstance instance;
    std::vector<VkQueueFlagBits> queue_bits;
    std::vector<std::string> device_extensions;
    std::vector<std::string> validation_layers;
};

class Device {
  public:
    VkPhysicalDevice physical = VK_NULL_HANDLE; // the used graphic card
    VkDevice logical; // the logical instance of the used graphic card
    
    std::vector<VkQueue> queues;
    
    bool is_ini = false;
    
    Device ();
    
    ~Device ();
    
    //! give VkInstance, needed queues, device extensions, (and maybe validation layers)
    bool ini (Device_ini ini);
    
    void pickPhysicalDevice (VkInstance, std::vector<std::string> device_extensions);
    
    uint32_t device_suitability (VkPhysicalDevice device, std::vector<std::string> device_extensions);
    
    bool check_extension_support (VkPhysicalDevice, std::vector<std::string> device_extensions);
};

#endif // Headder guard
