#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <fstream>

namespace SimpleEngine {

class Mesh {

public:

    std::string specifier;
    std::vector<uint8_t> vertexData;

    // THIS BAD - GLTF ALSO SUPPORTS 32-BIT INDICES...
    // FIX LATER
    std::vector<uint16_t> indexData;

    const static std::string importSavePath;

    Mesh();
    bool existsInImportCache();
    void saveToImportCache();
    void loadFromImportCache();

};

}

#endif