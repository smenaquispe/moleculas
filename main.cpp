#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>

#include "Molecule.h"
#include "MoleculeLoader.h"

float cameraDistance = 10.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= static_cast<float>(yoffset);
    if (cameraDistance < 1.0f) cameraDistance = 1.0f;
    if (cameraDistance > 100.0f) cameraDistance = 100.0f;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Visualizador de Moléculas", NULL, NULL);
    if (!window) {
        std::cerr << "Error al crear ventana" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetScrollCallback(window, scroll_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

    Molecule molecule;
    if (!molecule.loadFromFile("/home/smenaq/UNSA/grafica/molecula/data/data2.json")) {
        std::cerr << "Error al cargar la molécula" << std::endl;
        return -1;
    } else {
        std::cout << "Molécula cargada correctamente" << std::endl;
    }

    float cameraAngleX = 0.0f;
    float cameraAngleY = 0.0f;

    double lastX, lastY;
    bool firstMouse = true;
    glfwGetCursorPos(window, &lastX, &lastY);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float sensitivity = 0.005f;
            float offsetX = static_cast<float>(xpos - lastX) * sensitivity;
            float offsetY = static_cast<float>(ypos - lastY) * sensitivity;

            cameraAngleY += offsetX;
            cameraAngleX += offsetY;
        } else {
            firstMouse = true;
        }

        lastX = xpos;
        lastY = ypos;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -cameraDistance));
        view = glm::rotate(view, cameraAngleX, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, cameraAngleY, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        molecule.render(view, projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
