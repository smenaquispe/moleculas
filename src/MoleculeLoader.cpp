#include "MoleculeLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool MoleculeLoader::loadFromJSON(const std::string& filename, std::vector<Atom>& atoms, std::vector<Bond>& bonds) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return false;
    }

    json j;
    file >> j;

    try {
        auto compound = j["PC_Compounds"][0];

        auto atomIds = compound["atoms"]["aid"];
        auto elements = compound["atoms"]["element"];
        auto coords = compound["coords"][0]["conformers"][0];
        auto xCoords = coords["x"];
        auto yCoords = coords["y"];
        auto zCoords = coords["z"];

        atoms.clear();
        for (size_t i = 0; i < atomIds.size(); ++i) {
            atoms.emplace_back(atomIds[i], elements[i], xCoords[i], yCoords[i], zCoords[i]);
        }

        auto aid1 = compound["bonds"]["aid1"];
        auto aid2 = compound["bonds"]["aid2"];
        auto order = compound["bonds"]["order"];

        bonds.clear();
        for (size_t i = 0; i < aid1.size(); ++i) {
            bonds.emplace_back(aid1[i], aid2[i], order[i]);
        }

        std::cout << "Molécula cargada: " << atoms.size() << " átomos, "
                  << bonds.size() << " enlaces" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error al parsear JSON: " << e.what() << std::endl;
        return false;
    }
}
