
// #include <vector> // included by .hpp
// #include <string> // included by .hpp

#include "window_handler.hpp"

WindowHandler::WindowHandler () {
    
}

WindowHandler::~WindowHandler () {
    
}

bool WindowHandler::ini () {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // not use opengl
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // window with fixed size, (for now)
    
    // for fullscreen give a Monitor
    // glfw_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", glfwGetPrimaryMonitor(), nullptr);
    this->main_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    
    return true;
}

//! give a valide oppurtunity to access the events without hide the mainloop here

void WindowHandler::main_loop () {
    while (!glfwWindowShouldClose(this->main_window)) {
        glfwPollEvents();
    }
    
    // close window if thread gets out of mainloop
    glfwDestroyWindow(this->main_window);
    this->is_open = false;
    glfwTerminate();
}

// get the vk_extensions needed by glfw
std::vector<std::string> WindowHandler::get_vk_extensions () {
    uint32_t count = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
    std::vector<std::string> res(count);
    
    // convert char** to vector<string>
    for (int i = 0; i < count; i++) {
        res.push_back(glfwExtensions[i]);
    }
    return res;
}

//! check the whole glfw after multiwindow setup

void WindowHandler::glfw_setup () {
    // glfw init that must not repeated for second Window
}

void WindowHandler::glfw_cleanup () {
    // glfw cleanup that must be done after the last Window closes
}
