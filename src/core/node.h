#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>

class Node {

public:
    // quaternion rotation
    std::vector<double> rotation = { 0, 0, 0, 0 };
    std::vector<double> scale = { 1, 1, 1 };
    std::vector<double> translation = { 0, 0, 0 };

    float globalTransform[16];
    float localTransform[16];

    const Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;
    
    void updateLocalTransform();
    void updateGlobalTransform();

    Node();

    virtual ~Node() = default;
};

#endif