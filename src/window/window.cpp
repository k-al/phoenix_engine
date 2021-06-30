


// #include "window_handler.hpp" // included by .hpp
// #include "device.hpp" // included by .hpp

#include "window.hpp"

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
    
    this->window_handler.ini();
    
    this->create_vulkan_instance();
    
    Device_ini dev_ini;
    dev_ini.device_extensions = this->window_handler.glfw_get_vk_extensions();
    dev_ini.validation_layers = this->validation_layers;
    
    
    return true;
}

void Window::create_vulkan_instance () {
    
}
