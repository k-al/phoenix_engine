


// #include "window_handler.hpp" // included by .hpp
// #include "device.hpp" // included by .hpp

#include "window.hpp"

#include <iostream>

Window::Window () {}

Window::~Window () {}

// set the default validation_layers
void Window::set_validation_layers () {
    this->validation_layers = std::vector<std::string>({"VK_LAYER_KHRONOS_validation"});
}

bool Window::ini () {
    
    // if validation is enabled but there are no layers specified, choose the default layers
    if (this->enable_validation && this->validation_layers.empty()) {
        this->set_validation_layers();
    }
    
    this->window_handler.ini(WindowHandler_ini(this->validation_layers));
    
    std::vector<QueueBatch> queues = this->define_queues();
    
    Device_ini dev_ini;
        dev_ini.instance = this->window_handler.instance;
        dev_ini.device_extensions = this->window_handler.glfw_get_vk_extensions(true);
        dev_ini.validation_layers = this->validation_layers;
        dev_ini.queue_batches = &queues; //this->define_queues();
    
    this->device.ini(dev_ini);
    
    return true;
}

std::vector<QueueBatch> Window::define_queues () {
    std::vector<QueueBatch> res;
    
    {
        // get a graphics queue
        std::vector<std::function<bool(VkPhysicalDevice, VkQueueFamilyProperties, size_t fam_index)>> test_function;
        test_function.push_back( [](VkPhysicalDevice device, VkQueueFamilyProperties props, size_t fam_index)->bool{
            return props.queueFlags & VK_QUEUE_GRAPHICS_BIT;
        });
        
        // get a displaying queue
//         VkSurfaceKHR surface = this->window_handler.surface; // therefor get the display surface
        test_function.push_back( [/*surface*/](VkPhysicalDevice device, VkQueueFamilyProperties props, size_t fam_index)->bool{
            VkBool32 presentSupport = false;
//             vkGetPhysicalDeviceSurfaceSupportKHR(device, fam_index, surface, &presentSupport);
            return true;
        });
        
        res.push_back(QueueBatch(test_function));
    }
    
    return res;
}
