#include "Polygon.h"

Polygon::Polygon(std::vector<Vector3*> verts): _verts(verts){}
Polygon::~Polygon(){
    for(Vector3* vec: this->_verts)
        delete vec;
}
Vector3* Polygon::getVertice(int i){ return this->_verts.at(i); }