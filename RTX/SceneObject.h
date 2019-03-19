#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Material.h"
#include "Vector3.h"
#include "Ray.h"

class SceneObject{
private:
    Material* _mtl;
public:
    SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx);
    virtual ~SceneObject();
    Material* getMaterial() const;
    virtual bool intersect(Ray ray, float& ti){ return false; }
    virtual Vector3 getNormal(const Vector3 &hitPoint) { return Vector3(0,0,0); }
};

#endif