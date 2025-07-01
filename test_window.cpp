#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "glfwInit() falló\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);
    if (!window) {
        std::cerr << "glfwCreateWindow() falló\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    std::cout << "¡Ventana creada con éxito!\n";

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
