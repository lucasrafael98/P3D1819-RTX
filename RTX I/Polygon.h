#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "Vector3.h"

class Polygon{
private:
    std::vector<Vector3*> _verts;
public:
    Polygon(std::vector<Vector3*> verts);
    ~Polygon();

    Vector3* getVertice(int i);
};

#endif