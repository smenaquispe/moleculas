#ifndef MOLECULE_LOADER_H
#define MOLECULE_LOADER_H

#include <string>
#include <vector>
#include "Atom.h"
#include "Bond.h"

class MoleculeLoader {
public:
    static bool loadFromJSON(const std::string& filename, std::vector<Atom>& atoms, std::vector<Bond>& bonds);
};

#endif
