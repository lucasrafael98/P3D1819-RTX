#include "Sphere.h"

Sphere::Sphere(float r, float g, float b, float diff,
                float spec, float shine, float transm,
                float refidx, float x, float y, float z, float rad):
    SceneObject(r,g,b,diff,spec,shine,transm,refidx), _radius(rad){
        this->_pos = new Vector3(x,y,z);
    }

bool Sphere::intersect(Ray ray, float& t0, float &t1){
    ray.getDirection()->normalize();
    float d2 = (*(this->_pos) - *(ray.getOrigin())).length2();
    float b = ray.getDirection()->getX() * (this->_pos->getX() - ray.getOrigin()->getX())
                 + ray.getDirection()->getY() * (this->_pos->getY() - ray.getOrigin()->getY())
                 + ray.getDirection()->getZ() * (this->_pos->getZ() - ray.getOrigin()->getZ());
    if(d2 > (this->_radius * this->_radius)){
        if(b < 0) return false;
    }
    float r = b*b - d2 + (this->_radius * this->_radius);
    if(r < 0) return false;
    t0 = b - sqrt(r);
    t1 = b + sqrt(r);
    return true;
}
Vector3 Sphere::getNormal(const Vector3 &hitPoint){
    return (hitPoint - *(this->_pos)) / this->_radius;
}

float Sphere::getRadius(){ return this->_radius; }