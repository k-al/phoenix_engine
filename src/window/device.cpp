
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

// use only in context with test_unbatched
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
    this->found.assign(this->checks.size(), true);
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
    
    std::cout << "got here\n";
}

//! give needed queues, device extensions, (and maybe validation layers)
bool Device::ini (Device_ini ini) {
    
    this->device_extensions = ini.device_extensions;
    this->validation_layers = ini.validation_layers;
    
    if (this->physical == VK_NULL_HANDLE) {
        this->pick_physical(ini.instance);
    }
    
    this->logical_ini();
    
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
    
    // get the queues from the device
    uint32_t i = 0;
    for (auto batch : this->queue_batches) {
        this->queues.resize(this->queues.size() + batch->queue_indices.size()); //# i know its a resize in a loop
        for (uint32_t j = 0; j < batch->queue_indices.size(); ++j) {
            vkGetDeviceQueue(this->logical, batch->queue_indices[j].first, batch->queue_indices[j].second, &this->queues[i]);
            batch->queues[j] =&this->queues[i];
            ++i;
        }
    }
}

std::vector<VkDeviceQueueCreateInfo> Device::get_queue_infos (float* priority) {
    std::vector<bool> got_index;
    
    // keep track of how many queues are needed
    std::vector<size_t> queue_count;
    
    // get the nuber of supported queues
    uint32_t device_support_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(this->physical, &device_support_count, nullptr);
    
    // get a list of all supported queues
    std::vector<VkQueueFamilyProperties> device_support(device_support_count);
    vkGetPhysicalDeviceQueueFamilyProperties(this->physical, &device_support_count, device_support.data());
    
    queue_count.resize(device_support_count, 0); // initialize with no needed queue
    
    for (auto batch : this->queue_batches) { // get all batches
        // get the indices for full batches
        for (uint32_t i = 0; i < device_support_count; ++i) { // show them all queues
            if (batch->test_batched(device_support[i], i, queue_count[i])) {
                ++queue_count[i];
                break;
            }
        }
        
        // test if ready with batch
        if (batch->is_complete())
            continue;
        
        // get the single queues
        for (uint32_t i = 0; i < device_support_count; ++i) { // show them all queues
            if (batch->test_unbatched(device_support[i], i, queue_count[i])) {
                ++queue_count[i];
            }
        }
        
        // die if still not ready
        if (!batch->is_complete()) {
            std::cerr << "device_suitability said everything was supported, but wasnt\n";
            exit(1); // but device_suitability said everything was supported
        }
    }
    
    std::vector<VkDeviceQueueCreateInfo> queue_infos;
    
    for (uint32_t i = 0; i < device_support_count; ++i) {
        if (queue_count[i] == 0)
            continue;
        
        VkDeviceQueueCreateInfo queue_info{};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueFamilyIndex = i;  // wich queue should be created
            queue_info.queueCount = queue_count[i]; // how many
            queue_info.pQueuePriorities = priority; // priority from 0.0 to 1.0
        
        queue_infos.push_back(queue_info);
    }
    
    return queue_infos;
}

int32_t Device::check_queue_support (VkPhysicalDevice device) {
    // get the nuber of supported queues
    uint32_t device_support_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &device_support_count, nullptr);
    
    // get a list of all supported queues
    std::vector<VkQueueFamilyProperties> device_support(device_support_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &device_support_count, device_support.data());
    
    // first test if all requirements are supported
    for (auto batch : this->queue_batches) { // get all batches
        for (uint32_t i = 0; i < device_support_count; ++i) { // show them all queues
            batch->test_unbatched(device_support[i], i, 0);
        }
        if (!batch->is_complete()) {
            std::cout << "not all queues supported from " << this->queue_batches.size() << std::endl;
            return -1;
        }
        batch->test_reset();
    }
    
    // test if batches could use one queue and based on that give extra device score
    int32_t res = 0;
    for (auto batch : this->queue_batches) { // get all batches
        for (uint32_t i = 0; i < device_support_count; ++i) { // show them all queues
            if (batch->test_batched(device_support[i], i, 0)) {
                res += 50; //? make it depending on batch size
                break;
            }
        }
    }
    
    return res;
}


