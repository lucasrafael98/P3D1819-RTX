#include "Plane.h"

Plane::Plane(float r, float g, float b, float diff,
        float spec, float shine, float transm,
        float refidx, float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3):
        SceneObject(r,g,b,diff,spec,shine,transm,refidx){
            this->_pos1 = new Vector3(x1,y1,z1);
            this->_pos2 = new Vector3(x2,y2,z2);
            this->_pos3 = new Vector3(x3,y3,z3);
            Vector3 sub21 = *(this->_pos2) - *(this->_pos1);
            Vector3 sub31 = *(this->_pos3) - *(this->_pos1);
            Vector3 normal = sub21.cross(sub31);
            this->_normal = new Vector3(normal.getX(), normal.getY(), normal.getZ());
            this->_normal->normalize();
            this->setBBox(this->createBBox());
        }
Plane::~Plane(){
    delete this->_pos1;
    delete this->_pos2;
    delete this->_pos3;
    delete this->_normal;
}
Vector3* Plane::getPos1(){ return this->_pos1; }
Vector3* Plane::getPos2(){ return this->_pos2; }
Vector3* Plane::getPos3(){ return this->_pos3; }

Vector3 Plane::getNormal(const Vector3 &hitPoint){ return *(this->_normal); }

bool Plane::intersect(Ray ray, float& ti){
    float nd = this->_normal->dot((ray.getDirection()));
    if(fabs(nd) < EPSILON) return false;

    float t = - ((ray.getOrigin()) - *(this->_pos1)).dot(*(this->_normal)) / nd;
    if(t < 0.0f) return false;

    ti = t;
    return true;
}