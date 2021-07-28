
// #include <vector> // included by .hpp
// #include <string> // included by .hpp

#include <iostream>
#include <exception>
#include <string>

#include "c_helper.hpp"
#include "window_handler.hpp"

WindowHandler::WindowHandler () {
    
}

WindowHandler::~WindowHandler () {
    
    
    //? cleanup this->glfw_extensions i guess
}

bool WindowHandler::ini (WindowHandler_ini ini) {
    
    if (this->is_ini)
        return false;
    
    if (ini.validation_layers.size() > 0) {
        this->validation_layers = ini.validation_layers;
    } else {
        this->validation_layers = std::vector<std::string>();
    }
    
    // try to initialize everything
    try {
        this->glfw_ini();
        this->instance_ini();
        
    } catch (std::exception e) {
        std::cerr << e.what();
        return false;
    }
    
    this->is_ini = true;
    
    return true;
}

//! give a valide oppurtunity to access the events without hide the mainloop here

void WindowHandler::main_loop () {
    
    
    if (!this->is_ini || this->main_window == nullptr) {
        std::cerr << "error in WindowHandler::main_loop: initialize Window before use!\n";
        return;
    }
    
    while (!glfwWindowShouldClose(this->main_window)) {
        glfwPollEvents();
    }
    
    // close window if thread gets out of mainloop
    this->glfw_cleanup();
}

// get the vk_extensions needed by glfw
std::vector<std::string> WindowHandler::glfw_get_vk_extensions (bool as_vec) {
    if (this->glfw_extensions = nullptr) {
        this->glfw_extensions_count = 0;
        this->glfw_extensions = glfwGetRequiredInstanceExtensions(&this->glfw_extensions_count);
    }
    
    if (as_vec) {
        std::vector<std::string> res(this->glfw_extensions_count);
        
        // convert char** to vector<std::string>
        for (int i = 0; i < this->glfw_extensions_count; i++) {
            res.push_back(this->glfw_extensions[i]);
        }
        return res;
    } else {
        return std::vector<std::string>();
    }
    
    // ?!?
//     this->glfw_get_vk_extensions(true);
}

//! check after the whole glfw multiwindow setup

void WindowHandler::glfw_ini () {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // not use opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // window with fixed size, (for now)
    
    // for fullscreen give a Monitor
    // glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", glfwGetPrimaryMonitor(), nullptr);
    this->main_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    
    std::cout << "hey, initialized window\n" << this->main_window << std::endl;
}

void WindowHandler::glfw_cleanup () {
    glfwDestroyWindow(this->main_window);
    glfwTerminate();
}

void WindowHandler::instance_ini () {
    // Erst die eventuell benötigten validationLayers checken
    if (this->validation_layers.size() > 0 && !this->check_layer_support()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    
    // check extension support for the extensions needed by glfw
    if (!this->check_extension_support(this->glfw_get_vk_extensions(true))) {
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

    CstringArr validation_layers_wrap;
    validation_layers_wrap.set_array(this->validation_layers);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = this->glfw_extensions_count;
    createInfo.ppEnabledExtensionNames = this->glfw_extensions;
    
    std::cout << "got the glfw_extensions\n";
    
    //! got problem with wrapper
    createInfo.enabledLayerCount = validation_layers_wrap.length;
    createInfo.ppEnabledLayerNames = validation_layers_wrap.array; //? maybe need a .release()
        
    std::cout << "problem is in CreateInstanance\n";
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
    
    std::cout << "instance build\n;";
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

bool WindowHandler::check_layer_support () {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const std::string layerName : this->validation_layers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName.c_str(), layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}



