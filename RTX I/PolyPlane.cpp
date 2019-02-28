#include "PolyPlane.h"

PolyPlane::PolyPlane(std::vector<Vector3*> verts, std::vector<Vector3*> norms):
                    Polygon(verts), _norms(norms){}
PolyPlane::~PolyPlane(){
    for(Vector3* norm: this->_norms)
        delete norm;
}

Vector3* PolyPlane::getNormal(int i){ return this->_norms.at(i); }