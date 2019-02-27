#include "vector3.h"

Vector3::Vector3(float x, float y, float z):_x(x), _y(y), _z(z){}

float Vector3::getX(){ return this->_x; }
float Vector3::getY(){ return this->_y; }
float Vector3::getZ(){ return this->_z; }