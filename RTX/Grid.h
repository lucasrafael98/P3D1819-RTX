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
};

#endif