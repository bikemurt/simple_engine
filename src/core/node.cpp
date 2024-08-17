// core includes
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "node.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"
#include "bx/math.h"

Node::Node() {
    for (int i = 0; i < 3; i++) {
        rotation[i] = 0;
        scale[i] = 1;
        translation[i] = 0;
    }
    rotation[3] = 0;

    bx::mtxIdentity(rotationMatrix);
    bx::mtxIdentity(scaleMatrix);
    bx::mtxIdentity(translationMatrix);

    bx::mtxIdentity(localTransform);
    bx::mtxIdentity(globalTransform);
}

void Node::updateLocalTransform() {
    float tempMatrix[16];
    bx::mtxMul(tempMatrix, scaleMatrix, rotationMatrix);
    
    // finally, set the transform matrix
    bx::mtxMul(localTransform, tempMatrix, translationMatrix);
}

// local transform should be updated prior to calling this
void Node::updateGlobalTransform() {
    if (parent == nullptr) {
        for (int i = 0; i < 16; i++) {
            globalTransform[i] = localTransform[i];
        }
    } else {
        bx::mtxMul(globalTransform, localTransform, parent->globalTransform);
    }
    dirty = false;
}

void Node::setDirty() {
    if (!dirty) dirty = true;
}
void Node::setRotation(const double* r) {
    for (int i = 0; i < 4; i++) rotation[i] = r[i];
    bx::Quaternion rotation(rotation[0], rotation[1], rotation[2], rotation[3]);
    bx::mtxFromQuaternion(rotationMatrix, rotation);
    setDirty();
}

void Node::setScale(const double* s) {
    for (int i = 0; i < 3; i++) scale[i] = s[i];
    bx::mtxScale(scaleMatrix, scale[0], scale[1], scale[2]);
    setDirty();
}

void Node::setTranslation(const double* t, int8_t mask) {
    for (int i = 0; i < 3; i++) {
        if (
            (
                (1 << i) & mask
            ) == 0) continue;  
        translation[i] = t[i];
    }
    bx::mtxTranslate(translationMatrix, translation[0], translation[1], translation[2]);
    setDirty();
}

void Node::cleanTransformsProcess(Node* node) {
    node->updateGlobalTransform();
    for (std::unique_ptr<Node>& child : node->children) {
        cleanTransformsProcess(child.get());
    }
}

void Node::cleanTransforms() {
    // if this node is dirty, then clean it and all its child nodes
    if (dirty) {
        cleanTransformsProcess(this);
    }
}