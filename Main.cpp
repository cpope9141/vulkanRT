#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "WindowManager.h"

#include <iostream>

int main()
{
    Renderer renderer;

    createWindow();
    renderer.create();

    while (!windowShouldClose())
    {
        glfwPollEvents();
    }

    renderer.destroy();
    destroyWindow();

    return 0;
}