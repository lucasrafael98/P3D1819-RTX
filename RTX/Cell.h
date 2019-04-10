#ifndef CELL_H
#define CELL_H

#include "SceneObject.h"
#include "BBox.h"
#include <vector>

class Cell{
private:
    std::vector<SceneObject*> _objects;
    BBox* _bbox;
public:
    Cell();
    ~Cell();
    std::vector<SceneObject*> getObjects();
    BBox* getBBox();
    void addObject(SceneObject* object);
};

#endif