#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Material.h"
#include "Vector3.h"

// Describes a 3D object. A plane is ~sorta~ different so it doesn't inherit this.

class SceneObject{
private:
    Material* _mtl;
    Vector3* _pos;
public:
    SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx, float x, float y, float z);
    ~SceneObject();
    Material* getMaterial();
    Vector3* getPosition();
};

#endif