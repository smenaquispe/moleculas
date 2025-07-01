#ifndef MOLECULE_H
#define MOLECULE_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "Atom.h"
#include "Bond.h"

class Molecule {
public:
    Molecule();
    ~Molecule();

    bool loadFromFile(const std::string& filename);
    void render(const glm::mat4& view, const glm::mat4& projection);

private:
    std::vector<Atom> atoms;
    std::vector<Bond> bonds;

    GLuint sphereVAO, sphereVBO, sphereEBO;
    GLuint cylinderVAO, cylinderVBO, cylinderEBO;
    GLuint shaderProgram;

    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;
    std::vector<float> cylinderVertices;
    std::vector<unsigned int> cylinderIndices;

    void generateSphere(int stacks, int slices);
    void generateCylinder(int slices);

    void setupGL();
    void setupSphereVAO();
    void setupCylinderVAO();

    void renderBond(const Bond& bond);
    void cleanup();
};

#endif // MOLECULE_H
