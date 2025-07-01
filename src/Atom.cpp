#include "Atom.h"


void Atom::setElementProperties() {
    switch(element) {
        case 1: // Hidrógeno
            color = glm::vec3(1.0f, 1.0f, 1.0f);
            radius = 0.31f;
            break;
        case 6: // Carbono
            color = glm::vec3(0.3f, 0.3f, 0.3f);
            radius = 0.76f;
            break;
        case 8: // Oxígeno
            color = glm::vec3(1.0f, 0.0f, 0.0f);
            radius = 0.66f;
            break;
        default:
            color = glm::vec3(0.5f, 0.5f, 0.5f);
            radius = 0.5f;
    }
}