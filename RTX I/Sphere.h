#ifndef SPHERE_H
#define SPEHRE_H

#include "SceneObject.h"
#include "Vector3.h"

class Sphere:public SceneObject{
private:
    float _radius;
    Vector3* _pos;

public:

    Sphere(float r, float g, float b, float diff,
            float spec, float shine, float transm,
            float refidx, float x, float y, float z, float rad);

    float getRadius();
};

#endif