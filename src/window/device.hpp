// Headder guard
#ifndef VUL_DEVICE_HEADER
#define VUL_DEVICE_HEADER

#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <set>
#include <vector>
#include <functional>

// struct to specify queues
struct QueueBatch {
    QueueBatch () {};
    
    QueueBatch (std::vector<std::function<bool(VkQueueFamilyProperties, size_t fam_index)>> checks) {
        this->checks = checks;
    }
    
    void ini (std::vector<std::function<bool(VkQueueFamilyProperties, size_t fam_index)>> checks) {
        this->checks = checks;
    }
    
    std::vector<std::function<bool(VkQueueFamilyProperties, size_t fam_index)>> checks;
    std::vector<bool> found;
    std::vector<std::pair<size_t, size_t>> queue_indices; // pair of <family, index>
    std::vector<VkQueue*> queues;
    
    bool is_complete ();
    bool test_batched (VkQueueFamilyProperties, size_t fam_index, size_t number); //# unify to 'int test(vec<props>)' that batch as many as possible
    bool test_unbatched (VkQueueFamilyProperties, size_t fam_index, size_t number);
    void test_reset ();
    
};

struct Device_ini {
    VkInstance instance;
    std::vector<QueueBatch*> queue_batches;
    std::vector<std::string> device_extensions;
    std::vector<std::string> validation_layers;
};

class Device {
  public:
    VkPhysicalDevice physical = VK_NULL_HANDLE; // the used graphic card
    VkDevice logical; // the logical instance of the used graphic card
    
    std::vector<QueueBatch*> queue_batches;
    std::vector<VkQueue> queues;
    std::vector<std::string> device_extensions;
    std::vector<std::string> validation_layers;
    
    bool is_ini = false;
    
    Device ();
    
    ~Device ();
    
    //! give VkInstance, needed queues, device extensions, (and maybe validation layers)
    bool ini (Device_ini ini);
    
    ///////////////////////////
    // physical device
    ///////////////////////////
    
    void pick_physical (VkInstance);
    
    uint32_t device_suitability (VkPhysicalDevice);
    
    bool check_extension_support (VkPhysicalDevice);
    
    int32_t check_queue_support (VkPhysicalDevice);
    
    ///////////////////////////
    // logical device
    ///////////////////////////
    
    void logical_ini ();
    
    std::vector<VkDeviceQueueCreateInfo> get_queue_infos (float* priority);
    
};

#endif // Headder guard
