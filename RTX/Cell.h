#ifndef CELL_H
#define CELL_H

#include "SceneObject.h"
#include "BBox.h"
#include <vector>

class Cell{
private:
    std::vector<SceneObject*> _objects;
    std::vector<unsigned int> _ids;
    BBox* _bbox;
public:
    Cell();
    ~Cell();
    std::vector<SceneObject*> getObjects();
    BBox* getBBox();
    void addObject(SceneObject* object);
    void addID(unsigned int id);
    unsigned int getLastID(int i);
    void setLastID(int i, unsigned int newID);
};

#endif