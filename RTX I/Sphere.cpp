#include "Sphere.h"

Sphere::Sphere(float r, float g, float b, float diff,
                float spec, float shine, float transm,
                float refidx, float x, float y, float z, float rad):
    SceneObject(r,g,b,diff,spec,shine,transm,refidx), _radius(rad){
        this->_pos = new Vector3(x,y,z);
    }

float Sphere::getRadius(){ return this->_radius; }