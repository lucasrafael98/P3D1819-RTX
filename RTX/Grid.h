#ifndef GRID_H
#define GRID_H

#define M 2

#include "Cell.h"

class Grid{
private:
    std::vector<Cell*> _cells; // CELLS.
    BBox* _bbox;
    Vector3* _dimensions;
    Vector3* _cellnum;
public:
    Grid(std::vector<SceneObject*> objects);
    ~Grid();
    SceneObject* intersect(Ray ray, float &t0);
    void computeTnext(float dt, float &tnext, float &step, float n,
                        float &stop, float dir, float i, float tmin);
    SceneObject* traverseGrid(Ray ray, Vector3 i, Vector3 tnext, 
                                Vector3 step, Vector3 stop, Vector3 dt, float &t0);
};

#endif