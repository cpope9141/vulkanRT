#include "WindowManager.h"

#include <iostream>

const char* TITLE = "VulkanRenderer";

bool framebufferResized;
GLFWwindow* window;
VkSurfaceKHR surface;

static void errorCallback(int error, const char* msg);
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

void acknowledgeResize() { framebufferResized = false; }
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
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

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

        glfwShowWindow(window);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
    else
    {
        throw std::runtime_error("GLFW initialization fialed");
    }

    framebufferResized = false;
}

void destroySurface(VkInstance instance) { vkDestroySurfaceKHR(instance, surface, nullptr); }

void destroyWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

bool getFramebufferResized() { return framebufferResized; }
VkSurfaceKHR getSurface() { return surface; }
GLFWwindow* getWindow() { return window; }
bool windowShouldClose() { return glfwWindowShouldClose(window); }

static void errorCallback(int error, const char* msg) { std::cout << "GLFW error callback: " << msg << std::endl; }
static void framebufferResizeCallback(GLFWwindow* window, int width, int height) { framebufferResized = true; }