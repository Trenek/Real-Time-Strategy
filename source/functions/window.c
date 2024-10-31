#include "VulkanTools.h"

static const uint32_t WIDTH = 800;
static const uint32_t HEIGHT = 600;

GLFWwindow *createWindow(void) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    return glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", NULL, NULL);
}

void destroyWindow(GLFWwindow *window) {
    glfwDestroyWindow(window);

    glfwTerminate();
}
