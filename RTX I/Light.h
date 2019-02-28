#ifndef LIGHT_H
#define LIGHT_H
#include "Vector3.h"
#include "Color.h"

class Light {
private:
    Vector3* _pos;
    Color* _color;

public:
    Light(float x, float y, float z);
    Light(float x, float y, float z, float r, float g, float b);
    ~Light();

};

#endif