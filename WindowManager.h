#pragma once
#include "vulkan/vulkan_core.h"

const int HEIGHT = 600;
const int WIDTH = 800;

void createSurface(VkInstance instance);
void createWindow();
void destroySurface(VkInstance instance);
void destroyWindow();
VkSurfaceKHR getSurface();
bool windowShouldClose();