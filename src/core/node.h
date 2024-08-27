#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>

namespace SimpleEngine {

class Node {
private:
    bool dirty = false;

    float rotationMatrix[16];
    float scaleMatrix[16];
    float translationMatrix[16];

    double rotation[4];
    double scale[3];
    double translation[3];

    void setDirty();
    void cleanTransformsProcess(Node* node);

public:

    float globalTransform[16];
    float localTransform[16];

    const Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;
    
    void setRotation(const double* r);
    void setScale(const double* s);
    void setTranslation(const double* t, int8_t mask = 0b111);
    void cleanTransforms();

    void updateLocalTransform();
    void updateGlobalTransform();

    Node();

    virtual ~Node() = default;
};

}

#endif