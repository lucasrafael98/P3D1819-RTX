#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3{
private:
    float _x;
    float _y;
    float _z;

public:
    Vector3(float x, float y, float z);
    ~Vector3(){}

    float getX();
    float getY();
    float getZ();
};

#endif