#include "Sphere.h"

Sphere::Sphere(float r, float g, float b, float diff,
                float spec, float shine, float transm,
                float refidx, float x, float y, float z, float rad):
    SceneObject(r,g,b,diff,spec,shine,transm,refidx), _radius(rad){
        this->_pos = new Vector3(x,y,z);
    }

bool Sphere::intersect(Ray ray, float& t0, float &t1){
    Vector3 l = *(this->_pos) - *(ray.getOrigin());
    float tca = l.dot(*(ray.getDirection())); // Closest approach
    //std::cout << "TCA" << tca;
    if (tca < 0) return false; // Ray intersection behind ray origin
    float d2 = l.dot(l) - tca*tca;
    //std::cout << "D2" << d2;
    if (d2 > (_radius * _radius)) return false; // Ray doesn't intersect
    std::cout << "HIT";
    float thc = sqrt((_radius * _radius) - d2); // Closest approach to surface of sphere
    t0 = tca - thc;
    t1 = tca + thc;
    return true;
}
Vector3 Sphere::getNormal(const Vector3 &hitPoint){
    return (hitPoint - *(this->_pos)) / this->_radius;
}

float Sphere::getRadius(){ return this->_radius; }