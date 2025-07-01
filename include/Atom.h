#include <glm/gtc/type_ptr.hpp>

#ifndef ATOM_H
#define ATOM_H

struct Atom {
    int id;
    int element;
    glm::vec3 position;
    glm::vec3 color;
    float radius;
    
    Atom(int id, int element, float x, float y, float z) 
        : id(id), element(element), position(x, y, z) {
        setElementProperties();
    }
    
private:
    void setElementProperties();
};

#endif // ATOM_H