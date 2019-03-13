#include "Ray.h"

Ray::Ray(){ // default constructor
    this->_origin = new Vector3(0,0,0);
    this->_direction = new Vector3(0,0,1);
}
Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz){
    this->_origin = new Vector3(ox,oy,oz);
    this->_direction = new Vector3(dx,dy,dz);
}
Ray::Ray(Vector3 origin, Vector3 direction){
    this->_origin = new Vector3(origin.getX(),origin.getY(),origin.getZ());
    this->_direction = new Vector3(direction.getX(),direction.getY(),direction.getZ());
}
Ray::~Ray(){}
Vector3* Ray::getDirection(){ return this->_direction; }
Vector3* Ray::getOrigin(){ return this->_origin; }