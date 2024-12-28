#include <GLFW/glfw3.h>

#include "MY_ASSERT.h"

static const uint32_t WIDTH = 800;
static const uint32_t HEIGHT = 600;

static bool framebufferResized = false;

static void framebufferResizeCallback(GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height) {
    *(bool *)glfwGetWindowUserPointer(window) = true;
}

GLFWwindow *createWindow(bool **framebufferResizedPtr) {
    GLFWwindow *result = NULL;
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    result = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);

    glfwSetWindowUserPointer(result, &framebufferResized);
    glfwSetFramebufferSizeCallback(result, framebufferResizeCallback);
    *framebufferResizedPtr = &framebufferResized;

    return result;
}

void destroyWindow(GLFWwindow *window) {
    glfwDestroyWindow(window);

    glfwTerminate();
}
