
// #include <vector> // included by .hpp
#include <stdexcept>
#include <string>
#include <map>
// #include <set> // included by .hpp

// #define GLFW_INCLUDE_VULKAN // included by .hpp
// #include <GLFW/glfw3.h> // included by .hpp

#include "c_helper.hpp"

#include "device.hpp"

#include <iostream>

bool QueueBatch::is_complete () {
    if (this->found.empty())
        return false;
    bool complete = true;
    for (bool i : this->found) {
        complete = complete && i;
    }
    return complete;
}

bool QueueBatch::test_batched (VkQueueFamilyProperties props, size_t fam_index, size_t number) {
    if (this->checks.empty()) {
        std::cerr << "no queues specified!\n";
        return true;
    }
    
    for (auto check : this->checks) {
        if (!check(props, fam_index))
            return false;
    }
    
    this->queue_indices.resize(this->checks.size(), std::pair<size_t, size_t>(fam_index, number));
    return true;
}

bool QueueBatch::test_unbatched (VkQueueFamilyProperties props, size_t fam_index, size_t number) {
    if (this->checks.empty()) {
        std::cerr << "no queues specified!\n";
        return true;
    }
    
    bool res = false;
    this->found.resize(this->checks.size(), false); // if it was empty, fill with false (otherwise hold the data)
    this->queue_indices.resize(this->checks.size());
    
    for (size_t i = 0; i < this->found.size(); ++i) {
        if (!this->found[i] && this->checks[i](props, fam_index)) {
            this->queue_indices[i] = std::pair<size_t, size_t>(fam_index, number);
            this->found[i] = true;
            res = true;
        }
    }
    
    return res;
}

void QueueBatch::test_reset () {
    this->queue_indices.clear();
    this->found.clear();
}


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
    
    float priority = 1.0f;
    
    std::vector<VkDeviceQueueCreateInfo> queue_infos = this->get_queue_infos(&priority);
    
    // define all the device features, that are needed
    //! check what they do
    VkPhysicalDeviceFeatures deviceFeatures{};
    
    CstringArr c_val_layers;
    CstringArr c_device_extensions;
    if (!c_device_extensions.set_array(this->device_extensions)) {
        std::cerr << "CstringArr for device_extensions didnt get initialized!\n";
        exit(1);
    }
    
    // define the logical device, that should be created
    VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_infos.size());
        createInfo.pQueueCreateInfos = queue_infos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        // device specific extensions
        createInfo.enabledExtensionCount = c_device_extensions.length;
        createInfo.ppEnabledExtensionNames = c_device_extensions.array;
        
        if (this->validation_layers.size() > 0) { // the two fields are deprecated and not used in up-to-date implementations
            if (!c_val_layers.set_array(this->validation_layers)) {
                std::cerr << "CstringArr for validation_layers didnt get initialized!\n";
                exit(1);
            }
            
            createInfo.enabledLayerCount = c_val_layers.length; // set them anyway
            createInfo.ppEnabledLayerNames = c_val_layers.array;
        } else {
            createInfo.enabledLayerCount = 0;
        }
    
    // finaly create the logical device
    if (vkCreateDevice(this->physical, &createInfo, nullptr, &this->logical) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    
    this->queues.resize(this->queue_indices.size());
    
    // get the queues from the device
    for (size_t i = 0; i < this->queue_indices.size(); ++i) {
        vkGetDeviceQueue(this->logical, this->queue_indices[i].first, this->queue_indices[i].second, &this->queues[i]);
    }
}

std::vector<VkDeviceQueueCreateInfo> Device::get_queue_infos (float* priority) {
    std::vector<bool> got_index;
    
    // get the correct length
    this->queue_indices.resize(this->queue_req.size());
    got_index.resize(this->queue_req.size(), false);
    
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
                got_index[*set_it] = true;
            }
        }
    }
    
    // get the single queues
    for (size_t i = 0; i < this->queue_req.size(); ++i) {
        if (!got_index[i]) { // if it wasnt in one of the patches
            size_t index = 0;
            
            for (const auto supported : device_support) {
                if (queue_req[i] & supported.queueFlags == queue_req[i]) {
                    got_index[i] = true;
                    break;
                }
                ++index;
            }
            if (!got_index[i]) {
                //? throw
                exit(1); // but device_suitability said everything was supported
            }
            
            std::pair<size_t, size_t> queue_index;
            queue_index.first = index;
            
            if (queue_count.count(index) > 0) {
                ++queue_count[index];
            } else {
                queue_count[index] = 0;
            }
            queue_index.second = queue_count[index];
            
            this->queue_indices[i] = queue_index;
        }
    }
    
    std::vector<VkDeviceQueueCreateInfo> queue_infos;
    
    for (auto map_it = queue_count.begin(); map_it != queue_count.cend(); ++map_it) {
        // until here queue_count have the last index of each family
        // after incrementing it holds the actual number of queues
        ++map_it->second;
        
        VkDeviceQueueCreateInfo queue_info{};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueFamilyIndex = map_it->first;  // wich queue should be created
            queue_info.queueCount = map_it->second; // how many
            queue_info.pQueuePriorities = priority; // priority from 0.0 to 1.0
        
        queue_infos.push_back(queue_info);
    }
    
    return queue_infos;
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


