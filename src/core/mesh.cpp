// SIMPLE_ENGINE
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "mesh.h"

// MODULES
#include "fmt/format.h"

// STDLIB
#include <filesystem>
#include <fstream>

Mesh::Mesh() {

}

void Mesh::saveToImportCache() {
    std::string basePath = meshImportSavePath + specifier;

    std::ofstream outputFile(basePath + "-v.bin", std::ios::binary);
    outputFile.write(reinterpret_cast<const char*>(vertexData.data()), vertexData.size());
    outputFile.close();
    
    std::ofstream outputFile2(basePath + "-i.bin", std::ios::binary);
    outputFile2.write(reinterpret_cast<const char*>(indexData.data()), indexData.size());
    outputFile2.close();
}

void Mesh::loadFromImportCache() {
    std::string basePath = meshImportSavePath + specifier;

    std::string filepath = basePath + "-v.bin";
    std::ifstream inputFile(filepath, std::ios::binary | std::ios::ate);

    std::streamsize size = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    
    vertexData.resize(size);
    
    if (!inputFile.read(reinterpret_cast<char*>(vertexData.data()), size)) {
        fmt::println("Error reading file: " + filepath);
    }
    inputFile.close();
    
    std::string filepath2 = basePath + "-i.bin";
    std::ifstream inputFile2(filepath2, std::ios::binary | std::ios::ate);

    std::streamsize size2 = inputFile2.tellg();
    inputFile2.seekg(0, std::ios::beg);
    
    indexData.resize(size2);
    
    if (!inputFile2.read(reinterpret_cast<char*>(indexData.data()), size2)) {
        fmt::println("Error reading file: " + filepath2);
    }
    inputFile2.close();
}
