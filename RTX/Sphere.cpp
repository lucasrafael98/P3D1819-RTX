#include "Sphere.h"

Sphere::Sphere(float r, float g, float b, float diff,
                float spec, float shine, float transm,
                float refidx, float x, float y, float z, float rad):
    SceneObject(r,g,b,diff,spec,shine,transm,refidx), _radius(rad){
        this->_pos = new Vector3(x,y,z);
        std::cout << x << y << z << rad << std::endl;
    }

bool Sphere::intersect(Ray ray, float& ti){
    ray.getDirection()->normalize();
    float d2 = (*(this->_pos) - *(ray.getOrigin())).length2();
    float b = ray.getDirection()->getX() * (this->_pos->getX() - ray.getOrigin()->getX())
                 + ray.getDirection()->getY() * (this->_pos->getY() - ray.getOrigin()->getY())
                 + ray.getDirection()->getZ() * (this->_pos->getZ() - ray.getOrigin()->getZ());
    if (d2 > pow(this->_radius, 2) && b < 0) return false;
    //std::cout << "D2";
    float r = pow(b, 2) - d2 + pow(this->_radius, 2);
    if(r < 0) return false;
    //std::cout << "HIT";
    ti = b - sqrt(r);
    if(ti < 0.0f)
        ti = b + sqrt(r);
    return true;
}
Vector3 Sphere::getNormal(const Vector3 &hitPoint){
    return (hitPoint - *(this->_pos)) / this->_radius;
}

float Sphere::getRadius(){ return this->_radius; }