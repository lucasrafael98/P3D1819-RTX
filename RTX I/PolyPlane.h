#ifndef POLYPLANE_H
#define POLYPLANE_H

#include "Vector3.h"
#include "Polygon.h"
#include <vector>

class PolyPlane: public Polygon{
private:
    std::vector<Vector3*> _norms;
public:
    PolyPlane(float r, float g, float b, float diff,
                float spec, float shine, float transm,
                float refidx, std::vector<Vector3*> verts,
                std::vector<Vector3*> norms);
    ~PolyPlane();

    Vector3* getNormal(int i);
};

#endif