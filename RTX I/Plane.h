#ifndef PLANE_H
#define PLANE_H

#include "Vector3.h"
#include "SceneObject.h"

class Plane: public SceneObject{
private:
    Vector3 *_pos1, *_pos2, *_pos3, *_normal;
public:
    Plane(float r, float g, float b, float diff,
        float spec, float shine, float transm,
        float refidx, float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3);
    ~Plane();
    Vector3* getPos1();
    Vector3* getPos2();
    Vector3* getPos3();
    bool intersect(Ray ray, float& ti) override;
    Vector3 getNormal(const Vector3 &hitPoint) override;
};

#endif