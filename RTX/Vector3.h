#ifndef VECTOR3_H
#define VECTOR3_H

#define M_PI 3.1415926535f
#include <cmath>
#include <algorithm>
#include <iostream>
// for some intersection tests (defined in vec3 because it's included everywhere).
#define EPSILON 0.0000001f

class Vector3{
private:
    float _x;
    float _y;
    float _z;

public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(Vector3* vec3);
    ~Vector3(){}

    float getX() const;
    float getY() const;
    float getZ() const;
    void setX(float x);
    void setY(float y);
    void setZ(float z);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void normalize();
    void clamp(Vector3 min, Vector3 max);
    float dot(const Vector3 &v) const { return this->_x * v.getX() + this->_y * v.getY() + this->_z * v.getZ(); }
    Vector3 cross(const Vector3 &v){ return Vector3(this->_y * v.getZ() - this->_z * v.getY(), -(this->_x * v.getZ() - this->_z * v.getX()), this->_x * v.getY() - this->_y * v.getX()); }
    Vector3 operator - () const { return Vector3(-this->_x, -this->_y, -this->_z); }
    Vector3 operator + (const float &f) const { return Vector3(this->_x + f, this->_y + f, this->_z + f); }
    Vector3 operator - (const float &f) const { return Vector3(this->_x - f, this->_y - f, this->_z - f); }
    Vector3 operator * (const float &f) const { return Vector3(this->_x * f, this->_y * f, this->_z * f); }
    Vector3 operator / (const float &f) const { return Vector3(this->_x / f, this->_y / f, this->_z / f); }
    Vector3 operator + (const Vector3 &v) const { return Vector3(this->_x + v.getX(), this->_y + v.getY(), this->_z + v.getZ()); }
    Vector3 operator - (const Vector3 &v) const { return Vector3(this->_x - v.getX(), this->_y - v.getY(), this->_z - v.getZ()); }
    Vector3 operator * (const Vector3 &v)  const { return Vector3(this->_x * v.getX(), this->_y * v.getY(), this->_z * v.getZ()); }
    Vector3 operator / (const Vector3 &v)  const { return Vector3(this->_x / v.getX(), this->_y / v.getY(), this->_z / v.getZ()); }
    float length2() const { return this->_x * this->_x + this->_y * this->_y + this->_z * this->_z; }
    float length() const { return sqrt(length2()); }
};

#endif