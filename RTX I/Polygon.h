#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "Vector3.h"
#include "SceneObject.h"

class Polygon: public SceneObject{
private:
    std::vector<Vector3*> _verts;
    Vector3* _normal;
public:
    Polygon(float r, float g, float b, float diff,
            float spec, float shine, float transm,
            float refidx, std::vector<Vector3*> verts);
    ~Polygon();

    Vector3* getVertex(int i);
    Vector3 getNormal(const Vector3 &hitPoint) override;
    bool intersect(Ray ray, float& ti) override;
};

#endif