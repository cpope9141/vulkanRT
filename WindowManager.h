#pragma once
#include "vulkan/vulkan_core.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const int HEIGHT = 1080;
const int WIDTH = 1920;

void acknowledgeResize();
void createSurface(VkInstance instance);
void createWindow();
void destroySurface(VkInstance instance);
void destroyWindow();
bool getFramebufferResized();
VkSurfaceKHR getSurface();
GLFWwindow* getWindow();
bool windowShouldClose();