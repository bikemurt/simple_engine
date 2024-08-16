// core includes
#define ENABLE_DEBUG
#include "../utils/debug.h"

#include "node.h"

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/bx.h"
#include "bx/math.h"

void Node::updateTransform() {
    // FUTURE - do this more efficiently. only recalculate the matrices that have changed?

    bx::Quaternion rotation(rotation[0], rotation[1], rotation[2], rotation[3]);

    float scaleMatrix[16];
    bx::mtxIdentity(scaleMatrix);
    bx::mtxScale(scaleMatrix, scale[0], scale[1], scale[2]);

    float rotationMatrix[16];
    bx::mtxFromQuaternion(rotationMatrix, rotation);

    float translationMatrix[16];
    bx::mtxIdentity(translationMatrix);
    bx::mtxTranslate(translationMatrix, translation[0], translation[1], translation[2]);

    float tempMatrix[16];
    bx::mtxMul(tempMatrix, rotationMatrix, scaleMatrix);
    
    // finally, set the transform matrix
    bx::mtxMul(transform, translationMatrix, tempMatrix);

}