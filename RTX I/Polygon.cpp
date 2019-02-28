#include "Polygon.h"

Polygon::Polygon(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx, std::vector<Vector3*> verts):
                 SceneObject(r,g,b,diff,spec,shine,transm,refidx),
                  _verts(verts){}
Polygon::~Polygon(){
    for(Vector3* vec: this->_verts)
        delete vec;
}
Vector3* Polygon::getVertice(int i){ return this->_verts.at(i); }