#include "Polygon.h"

Polygon::Polygon(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx, std::vector<Vector3*> verts):
                 SceneObject(r,g,b,diff,spec,shine,transm,refidx),
                  _verts(verts){
                        Vector3 sub21 = *(this->_verts.at(1)) - *(this->_verts.at(0));
                        Vector3 sub31 = *(this->_verts.at(2)) - *(this->_verts.at(0));
                        Vector3 normal = sub21.cross(sub31);
                        this->_normal = new Vector3(normal.getX(), normal.getY(), normal.getZ());
                        this->_normal->normalize();
                  }
Polygon::~Polygon(){
    for(Vector3* vec: this->_verts)
        delete vec;
}
Vector3* Polygon::getVertice(int i){ return this->_verts.at(i); }
Vector3 Polygon::getNormal(const Vector3 &hitPoint){ return *(this->_normal); }
bool Polygon::intersect(Ray ray, float& ti){

    return true;
}