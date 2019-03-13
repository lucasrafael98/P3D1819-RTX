#include "PolyPlane.h"

PolyPlane::PolyPlane(float r, float g, float b, float diff,
                float spec, float shine, float transm,
                float refidx, std::vector<Vector3*> verts,
                std::vector<Vector3*> norms):
                    Polygon(r,g,b,diff,spec,shine,transm,refidx,verts), _norms(norms){}
PolyPlane::~PolyPlane(){
    for(Vector3* norm: this->_norms)
        delete norm;
}

Vector3* PolyPlane::getNormal(int i){ return this->_norms.at(i); }