#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <fstream>

class Mesh {

private:
    std::string meshImportSavePath = "C:\\Projects\\CppTesting\\simple_engine\\imports\\meshes\\";

public:

    std::string specifier;
    std::vector<uint8_t> vertexData;
    std::vector<uint16_t> indexData;

    Mesh();
    void saveToImportCache();
    void loadFromImportCache();

};

#endif