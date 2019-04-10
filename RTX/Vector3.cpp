#include "Vector3.h"

Vector3::Vector3():_x(0), _y(0), _z(0){}
Vector3::Vector3(float x, float y, float z):_x(x), _y(y), _z(z){}
Vector3::Vector3(Vector3* vec3): _x(vec3->getX()),_y(vec3->getY()),_z(vec3->getZ()){}

float Vector3::getX() const{ return this->_x; }
float Vector3::getY() const{ return this->_y; }
float Vector3::getZ() const{ return this->_z; }
void Vector3::setX(float x) { this->_x = x; }
void Vector3::setY(float y) { this->_y = y; }
void Vector3::setZ(float z) { this->_z = z; }

void Vector3::rotateX(float angle){
    float y = this->_y; float z = this->_z;
    this->_y = y * cos(angle) - z * sin(angle);
    this->_z = y * sin(angle) + z * cos(angle);
}
void Vector3::rotateY(float angle){
    float x = this->_x; float z = this->_z;
    this->_x = z * cos(angle) - x * sin(angle);
    this->_z = z * sin(angle) + x * cos(angle);
}
void Vector3::rotateZ(float angle){
    float x = this->_x; float y = this->_y;
    this->_x = x * cos(angle) - y * sin(angle);
    this->_y = x * sin(angle) + y * cos(angle);   
}
void Vector3::normalize(){
    float norm2 = this->_x * this->_x + this->_y * this->_y + this->_z * this->_z;
    if(norm2 > 0) {
        float invNorm = 1 / sqrt(norm2);
        this->_x *= invNorm, this->_y *= invNorm, this->_z *= invNorm;
    }
}