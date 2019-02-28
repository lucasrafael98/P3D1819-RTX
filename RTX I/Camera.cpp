#include "Camera.h"

Camera::Camera(int resX, int resY){
    this->_resX = resX;
    this->_resY = resY;
}
Camera::~Camera(){}

int Camera::getResX(){return this->_resX;}
int Camera::getResY(){return this->_resY;}