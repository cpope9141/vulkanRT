#include "WindowManager.h"

#include <iostream>

/*
constants
*/
const char* TITLE = "VulkanRenderer";

/*
variables
*/
GLFWwindow* window;
VkSurfaceKHR surface;

/*
function declarations
*/
static void errorCallback(int error, const char* msg);

/*
functions
*/
void createSurface(VkInstance instance)
{
    if (VK_SUCCESS != glfwCreateWindowSurface(instance, window, nullptr, &surface)) {
        throw std::runtime_error("Failed to create window surface");
    }
}

void createWindow() 
{
    int width, height;

    glfwSetErrorCallback(errorCallback);

    if (glfwInit())
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

        if (nullptr != window)
        {
            glfwGetWindowSize(window, &width, &height);
            const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowPos(window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);
        }
        else
        {
            throw std::runtime_error("GLFW failed to create a window");
        }
    }
    else
    {
        throw std::runtime_error("GLFW initialization fialed");
    }
}

void destroySurface(VkInstance instance) { vkDestroySurfaceKHR(instance, surface, nullptr); }

void destroyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

VkSurfaceKHR getSurface() { return surface; }
GLFWwindow* getWindow() { return window; }
bool windowShouldClose() { return glfwWindowShouldClose(window); }

static void errorCallback(int error, const char* msg) { std::cout << "GLFW error callback: " << msg << std::endl; }