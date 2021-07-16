
// #include <vector> // included by .hpp
#include <stdexcept>
#include <string>
#include <map>
// #include <set> // included by .hpp

// #define GLFW_INCLUDE_VULKAN // included by .hpp
// #include <GLFW/glfw3.h> // included by .hpp

#include "device.hpp"

#include <iostream>


Device::Device () {}

Device::~Device () {
    
    // claenup the logical Device
    if (this->is_ini) {
        vkDestroyDevice(this->logical, nullptr);
    }
}

//! give needed queues, device extensions, (and maybe validation layers)
bool Device::ini (Device_ini ini) {
    
    this->device_extensions = ini.device_extensions;
    this->validation_layers = ini.validation_layers;
    
    if (this->physical == VK_NULL_HANDLE) {
        this->pick_physical(ini.instance);
    }
    
    
    this->is_ini = true;
    return true;
}

// select "the best" GPU and assign it to the physical member
//? make it generic, so you can pick different GPUs
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
    
    VkPhysicalDeviceProperties deviceProperties; // info about basic device properties (name, type, vulkan-version)
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    
    VkPhysicalDeviceFeatures deviceFeatures; // info about optional features (64-bit floats, multi-viewport rendering)
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    // graphic card must have all required queueFamilies
    score += this->check_queue_support(device);
    if (score == 0) { // check_extension_support returns -1 if something is unavailable so if score is 0 after call, device is not suitable
        return 0;
    }
    
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
    
    // get the correct length
    this->queue_indices.reserve(this->queue_req.size());
    
    std::map<size_t, size_t> queue_count;
    
    // get the nuber of supported queues
    uint32_t device_support_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(this->physical, &device_support_count, nullptr);
    
    // get a list of all supported queues
    std::vector<VkQueueFamilyProperties> device_support(device_support_count);
    vkGetPhysicalDeviceQueueFamilyProperties(this->physical, &device_support_count, device_support.data());
    
    // get the indices for full batches
    for (const std::set<size_t> batch : this->queue_batch) {
        bool found = false;
        uint32_t batch_flags = 0;
        size_t index = 0;
        
        // get all flags of the batch together
        for (auto set_it = batch.begin(); set_it != batch.end(); set_it++) {
            batch_flags = batch_flags | this->queue_req[*set_it];
        }
        // test the whole flagset
        for (const auto supported : device_support) {
            if (batch_flags & supported.queueFlags == batch_flags) {
                found = true;
                break;
            }
            ++index;
        }
        if (found) {
            std::pair<size_t, size_t> queue_index;
            queue_index.first = index;
            
            if (queue_count.count(index) > 0) {
                ++queue_count[index];
            } else {
                queue_count[index] = 0;
            }
            queue_index.second = queue_count[index];
            
            for (auto set_it = batch.begin(); set_it != batch.end(); set_it++) {
                this->queue_indices[*set_it] = queue_index;
            }
        }
    }
    
    // until here queue_count have the last index of each family
    // after incrementing it holds the actual number of queues
    
    
    
    /*
    // define all the queues, that are needed
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    float queuePriority = 1.0f;
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    // because there are more than one queues, its an array wich is build in this loop
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;  // wich queue should be created
            queueCreateInfo.queueCount = 1; // how many
            queueCreateInfo.pQueuePriorities = &queuePriority; // priority from 0.0 to 1.0
        
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    // define all the device features, that are needed
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    // define the logical device, that should be created
    VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        // device specific extensions
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        
        if (enableValidationLayers) { // the two fields are deprecated and not used in up-to-date implementations
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); // set them anyway
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }
    
    // finaly create the logical device
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    // get the queues from the device
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue); // get the 0'th graphic queue
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue); // get the 0'th presentation queue
    */
}

int32_t Device::check_queue_support (VkPhysicalDevice device) {
    std::cout << "check_queue_support in\n";
    
    // get the nuber of supported queues
    uint32_t device_support_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &device_support_count, nullptr);
    
    // get a list of all supported queues
    std::vector<VkQueueFamilyProperties> device_support(device_support_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &device_support_count, device_support.data());
    
    // first test if all requirements are supported
    for (const auto requirement : this->queue_req) {
        bool found = false;
        
        for (const auto supported : device_support) {
            if (requirement & supported.queueFlags) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "check_queue_support out\n";
            return -1;
        }
    }
    
    // test if batches could use one queue and based on that give extra device score
    int32_t res = 0;
    for (const std::set<size_t> batch : this->queue_batch) {
        bool found = false;
        uint32_t batch_flags = 0;
        
        // get all flags of the batch together
        for (auto set_it = batch.begin(); set_it != batch.end(); set_it++) {
            batch_flags = batch_flags | this->queue_req[*set_it];
        }
        // test the whole flagset
        for (const auto supported : device_support) {
            if (batch_flags & supported.queueFlags == batch_flags) {
                found = true;
                break;
            }
        }
        if (found) {
            res += batch.size() * 20;
        }
    }
    std::cout << "check_queue_support out\n";
    
    return res;
}


