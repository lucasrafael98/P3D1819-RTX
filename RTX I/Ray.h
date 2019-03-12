#ifndef RAY_H
#define RAY_H

#include <iostream>
#include <string>
#include <fstream>
#include "Vector3.h"

class Ray{
private:
    Vector3 *_origin, *_direction;
public:

    Ray();
    Ray(float ox, float oy, float oz, float dx, float dy, float dz);
    ~Ray();

};

#endif