
// #include <vector> // included by .hpp
#include <stdexcept>
#include <string>
#include <set>

// #define GLFW_INCLUDE_VULKAN // included by .hpp
// #include <GLFW/glfw3.h> // included by .hpp

#include "device.hpp"


Device::Device () {}

Device::~Device () {
    
    // claenup the logical Device
    if (this->is_ini) {
        vkDestroyDevice(this->logical, nullptr);
    }
}

//! give needed queues, device extensions, (and maybe validation layers)
bool Device::ini (Device_ini ini) {
    
    this->device_extensions = *ini.device_extensions;
    this->validation_layers = *ini.validation_layers;
    
    if (this->physical == VK_NULL_HANDLE) {
        this->pick_physical(ini.instance);
    }
    
    
    this->is_ini = true;
    return true;
}

// select "the best" GPU and assign it to the physical member
void Device::pick_physical (VkInstance instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr); // get the number of graphic carts with Vulkan-support
    
    if (deviceCount == 0) { // throw error if no graphic cart availible
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()); // get a vector of all graphic carts
    
    // pick the card with the highest rating
    uint32_t hiscore = 0;
    uint32_t score = 0;
    for (const VkPhysicalDevice device : devices) {
        score = this->device_suitability(device);
        if (score > hiscore) {
            this->physical = device;
            hiscore = score;
        }
    }
    
    if (hiscore == 0) { // couldnt find a graphic cart, wich can compute what was needed
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

// rate one GPU (mainly check if all is supported)
uint32_t Device::device_suitability (VkPhysicalDevice device) {
    
    uint32_t score = 1;
    
    // QueueFamilyIndices qf_indices = findQueueFamilies(device); // get the required queueFamilies
    
    VkPhysicalDeviceProperties deviceProperties; // info about basic device properties (name, type, vulkan-version)
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    
    VkPhysicalDeviceFeatures deviceFeatures; // info about optional features (64-bit floats, multi-viewport rendering)
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    // graphic card must have all required queueFamilies
//     if (!qf_indices.isComplete()) {
//         return 0;
//     }
    
    // graphic card must support all required extensions
    if (!this->check_extension_support(device)) {
        return 0;
    }
    
    // get the swapchain-info only if the swapchain extension is supported
//     SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
    
    // swapchain must support at least something:
//     if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty()) {
//         return 0;
//     }
    
    // now the actual rating:
    
    // discrete gpus are mostly much better than intigrated gpus
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    
    return score;
}

// checks if all device specific extensions (stored in 'deviceExtensions') are supported by given device 
bool Device::check_extension_support (VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    
    // get the supported extensions from this device
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
    
    std::set<std::string> required_extensions (this->device_extensions.begin(), this->device_extensions.end());
    
    // delete all found extensions from the set of wanted extension
    for (const auto& extension : availableExtensions) {
        required_extensions.erase(extension.extensionName);
    }
    
    // if something remain in the set, its not supported
    return required_extensions.empty();
}

void Device::logical_ini () {
    
}


