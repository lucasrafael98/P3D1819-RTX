#include "Ray.h"
#include <mutex>

static unsigned int rayID = 1;
std::mutex mtx_rayid;

Ray::Ray(){ // default constructor
    this->_origin = Vector3(0,0,0);
    this->_direction = Vector3(0,0,1);
    this->_id = rayID;
    rayID++;
}
Ray::Ray(float ox, float oy, float oz, float dx, float dy, float dz){
    this->_origin = Vector3(ox,oy,oz);
    this->_direction = Vector3(dx,dy,dz);
    this->_id = rayID;
    rayID++;
}
Ray::Ray(Vector3 origin, Vector3 direction){
    this->_origin = Vector3(origin.getX(),origin.getY(),origin.getZ());
    this->_direction = Vector3(direction.getX(),direction.getY(),direction.getZ());
    mtx_rayid.lock();
    this->_id = rayID;
    rayID++;
    mtx_rayid.unlock();
}
Ray::~Ray(){}
Vector3 Ray::getDirection(){ return this->_direction; }
Vector3 Ray::getOrigin(){ return this->_origin; }
unsigned int Ray::getID(){ return this->_id; }