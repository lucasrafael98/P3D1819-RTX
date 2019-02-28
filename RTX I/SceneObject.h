#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Material.h"
#include "Vector3.h"

class SceneObject{
private:
    Material* _mtl;
public:
    SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx);
    ~SceneObject();
    Material* getMaterial();
};

#endif