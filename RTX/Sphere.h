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
    ~Sphere(){ delete _pos; }
    float getRadius();
    bool intersect(Ray ray, float& ti) override;
    Vector3 getNormal(const Vector3 &hitPoint) override;
    BBox* createBBox() override;
    static void printTotalIntersections();
};

#endif