#include "Camera.h"

Camera::Camera(float fromx, float fromy, float fromz,
        float atx, float aty, float atz,
        float upx, float upy, float upz,
        float resx, float resy,
        float angle, float hither):
        _resx(resx), _resy(resy), _angle(angle), _hither(hither){
            this->_from = new Vector3(fromx, fromy, fromz);
            this->_at = new Vector3(atx, aty, atz);
            this->_up = new Vector3(upx, upy, upz);
        }
Camera::~Camera(){}

Vector3* Camera::getFrom(){ return this->_from;}
Vector3* Camera::getAt(){ return this->_at;}
Vector3* Camera::getUp(){ return this->_up;}
float Camera::getResX(){ return this->_resx;}
float Camera::getResY(){ return this->_resy;}
float Camera::getAngle(){ return this->_angle;}
float Camera::getHither(){ return this->_hither;}