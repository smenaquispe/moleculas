#include "Molecule.h"
#include "MoleculeLoader.h"
#include "ShaderUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Molecule::Molecule() {
    generateSphere(20, 20);
    generateCylinder(20);
    setupGL();
}

Molecule::~Molecule() {
    cleanup();
}

bool Molecule::loadFromFile(const std::string& filename) {
    return MoleculeLoader::loadFromJSON(filename, atoms, bonds);
}

void Molecule::render(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(sphereVAO);
    for (const auto& atom : atoms) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), atom.position);
        model = glm::scale(model, glm::vec3(atom.radius));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(atom.color));
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(cylinderVAO);
    for (const auto& bond : bonds) {
        renderBond(bond);
    }
}

void Molecule::generateSphere(int stacks, int slices) {
    sphereVertices.clear();
    sphereIndices.clear();

    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * i / stacks;
        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * j / slices;

            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);

            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);
            
            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            sphereIndices.push_back(first);
            sphereIndices.push_back(second);
            sphereIndices.push_back(first + 1);

            sphereIndices.push_back(second);
            sphereIndices.push_back(second + 1);
            sphereIndices.push_back(first + 1);
        }
    }
}

void Molecule::generateCylinder(int slices) {
    cylinderVertices.clear();
    cylinderIndices.clear();

    for (int i = 0; i <= slices; ++i) {
        float theta = 2.0f * M_PI * i / slices;
        float x = cos(theta);
        float z = sin(theta);

        // base
        cylinderVertices.push_back(x);
        cylinderVertices.push_back(0.0f);
        cylinderVertices.push_back(z);
        cylinderVertices.push_back(x);
        cylinderVertices.push_back(0.0f);
        cylinderVertices.push_back(z);

        // top
        cylinderVertices.push_back(x);
        cylinderVertices.push_back(1.0f);
        cylinderVertices.push_back(z);
        cylinderVertices.push_back(x);
        cylinderVertices.push_back(0.0f);
        cylinderVertices.push_back(z);
    }

    for (int i = 0; i < slices; ++i) {
        int base = i * 2;

        cylinderIndices.push_back(base);
        cylinderIndices.push_back(base + 2);
        cylinderIndices.push_back(base + 1);

        cylinderIndices.push_back(base + 1);
        cylinderIndices.push_back(base + 2);
        cylinderIndices.push_back(base + 3);
    }
}

void Molecule::setupGL() {
    std::string vertexSource = loadShaderSource("/home/smenaq/UNSA/grafica/molecula/shaders/vertex.glsl");
    std::string fragmentSource = loadShaderSource("/home/smenaq/UNSA/grafica/molecula/shaders/fragment.glsl");

    shaderProgram = createShaderProgram(vertexSource.c_str(), fragmentSource.c_str());

    setupSphereVAO();
    setupCylinderVAO();
}

void Molecule::setupSphereVAO() {
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(unsigned int), sphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Molecule::setupCylinderVAO() {
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glGenBuffers(1, &cylinderEBO);

    glBindVertexArray(cylinderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(float), cylinderVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinderIndices.size() * sizeof(unsigned int), cylinderIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Molecule::renderBond(const Bond& bond) {
    const Atom* atom1 = nullptr;
    const Atom* atom2 = nullptr;

    for (const auto& atom : atoms) {
        if (atom.id == bond.atom1) atom1 = &atom;
        if (atom.id == bond.atom2) atom2 = &atom;
    }

    if (!atom1 || !atom2) return;

    glm::vec3 direction = atom2->position - atom1->position;
    float length = glm::length(direction);
    direction = glm::normalize(direction);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), atom1->position);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (glm::abs(glm::dot(direction, up)) > 0.99f) {
        up = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 right = glm::normalize(glm::cross(direction, up));
    up = glm::cross(right, direction);

    glm::mat4 rotation(1.0f);
    rotation[0] = glm::vec4(right, 0.0f);
    rotation[1] = glm::vec4(direction, 0.0f);
    rotation[2] = glm::vec4(up, 0.0f);

    model *= rotation;
    model = glm::scale(model, glm::vec3(0.1f * bond.order, length, 0.1f * bond.order));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shaderProgram, "color"), 0.5f, 0.5f, 0.5f);

    glDrawElements(GL_TRIANGLES, cylinderIndices.size(), GL_UNSIGNED_INT, 0);
}

void Molecule::cleanup() {
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);
    glDeleteBuffers(1, &cylinderEBO);
    glDeleteProgram(shaderProgram);
}
