#ifndef NODE3D_H
#define NODE3D_H

#include <vector>

class Node {

public:
    // quaternion rotation
    std::vector<double> rotation = { 0, 0, 0, 0 };
    std::vector<double> scale = { 1, 1, 1 };
    std::vector<double> translation = { 0, 0, 0 };

    float transform[16];

    int meshIndex;

    bool updateTransformOnRenderFrame = true;

    std::vector<Node> children;
    
    void updateTransform();
};

#endif