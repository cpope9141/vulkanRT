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

        renderer.prepare();
        renderer.submit();
    }

    renderer.destroy();
    destroyWindow();

    return 0;
}