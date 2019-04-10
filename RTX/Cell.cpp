#include "Cell.h"

// empty constructor because we need to add objects later
Cell::Cell(){}
Cell::~Cell(){}

std::vector<SceneObject*> Cell::getObjects(){
    return this->_objects;
}
BBox* Cell::getBBox(){
    return this->_bbox;
}
void Cell::addObject(SceneObject* object){
    this->_objects.push_back(object);
}