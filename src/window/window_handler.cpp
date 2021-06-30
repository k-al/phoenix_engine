
// #include <vector> // included by .hpp
// #include <string> // included by .hpp

#include <iostream>
#include <exception>
#include <string>
#include <cstring>

#include "window_handler.hpp"


WindowHandler::WindowHandler () {
    
}

WindowHandler::~WindowHandler () {
    
}

bool WindowHandler::ini () {
    
    // try to initialize everything
    try {
        this->glfw_ini();
        
        
        
    } catch (std::exception e) {
        std::cerr << e.what();
        return false;
    }
    
    return true;
}

//! give a valide oppurtunity to access the events without hide the mainloop here

void WindowHandler::main_loop () {
    while (!glfwWindowShouldClose(this->main_window)) {
        glfwPollEvents();
    }
    
    // close window if thread gets out of mainloop
    this->glfw_cleanup();
}

// get the vk_extensions needed by glfw
std::vector<std::string> WindowHandler::glfw_get_vk_extensions () {
    uint32_t count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);
    std::vector<std::string> res(count);
    
    // convert char** to vector<std::string>
    for (int i = 0; i < count; i++) {
        res.push_back(glfwExtensions[i]);
    }
    return res;
}

//! check after the whole glfw multiwindow setup

void WindowHandler::glfw_ini () {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // not use opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // window with fixed size, (for now)
    
    // for fullscreen give a Monitor
    // glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", glfwGetPrimaryMonitor(), nullptr);
    this->main_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void WindowHandler::glfw_cleanup () {
    glfwDestroyWindow(this->main_window);
    this->is_open = false;
    glfwTerminate();
}

void WindowHandler::instance_ini () {
    // Erst die eventuell benötigten validationLayers checken
    if (this->validation_layers.size() > 0 && !this->check_layer_support()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    
    std::vector<char*> extensions = this->glfw_get_vk_extensions();
    
    // check extension support for the extensions needed by glfw
    if (!this->check_extension_support(extensions)) {
        throw std::runtime_error("Missing a global extension needed by glfw!");
    }
    
    // Übergabeparameter für CreateInstanance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Tarnish";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "tarnish-engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;
        

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = const_cast<char**>(extensions.data());
    createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    createInfo.ppEnabledLayerNames = const_cast<char**>(validation_layers.data());
        
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

bool WindowHandler::check_extension_support(std::vector<std::string> required) {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);              // wie viele Extensions gibt es?
    std::vector<VkExtensionProperties> extensions(extensionCount);                          // entsprechend großes Array erstellen
    
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());    // Die Extensions ins Array füllen
    
    // Ausgabe aller available Extensions
    if (this->validation_layers.size() > 0) {
        std::cout << "available extensions(" << extensionCount << "):\n";
        for (const VkExtensionProperties extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
    }
    
    // Der eigentliche check
    bool haveit;
    for (const std::string needed: required) {
        haveit = false;
        for (const VkExtensionProperties available: extensions) {
            if (strcmp(needed.c_str(), available.extensionName) == 0) {
                haveit = true;
                break;
            }
        }
        if (!haveit) {
            std::cerr << "Missing a global extension '" << needed << "' needed by glfw!\n";
            return false;
        }
    }
    return true;
}



