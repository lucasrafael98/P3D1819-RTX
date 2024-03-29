#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <string>
#include <fstream>
#include "Vector3.h"

class Ray{
private:
    Vector3 _origin, _direction;
    unsigned int _id;
public:

    Ray();
    Ray(float ox, float oy, float oz, float dx, float dy, float dz);
    Ray(Vector3 origin, Vector3 direction);
    ~Ray();

    Vector3 getDirection();
    Vector3 getOrigin();
    unsigned int getID();

};

#endif