#include "Camera.h"

Camera::Camera(float fromx, float fromy, float fromz,
        float atx, float aty, float atz,
        float upx, float upy, float upz,
        float resx, float resy,
        float angle, float hither):
        _resx(resx), _resy(resy), _fov(angle), _near(hither){
            this->_eye = new Vector3(fromx, fromy, fromz);
            this->_at = new Vector3(atx, aty, atz);
            this->_up = new Vector3(upx, upy, upz);
        }
Camera::~Camera(){}
// TODO: is nff angle an fov? is hither camera near?

Vector3* Camera::getEye(){ return this->_eye;}
Vector3* Camera::getAt(){ return this->_at;}
Vector3* Camera::getUp(){ return this->_up;}
float Camera::getResX(){ return this->_resx;}
float Camera::getResY(){ return this->_resy;}
float Camera::getFOV(){ return this->_fov;}
float Camera::getNear(){ return this->_near;}

Vector3* Camera::computeFromAtAngle(){
    Vector3 fromx = Vector3(this->_eye->getX(), 0, 0);
    Vector3 fromy = Vector3(0, this->_eye->getY(), 0);
    Vector3 fromz = Vector3(0, 0, this->_eye->getZ());
    Vector3 atx = Vector3(this->_at->getX(), 0, 0);
    Vector3 aty = Vector3(0, this->_at->getY(), 0);
    Vector3 atz = Vector3(0, 0, this->_at->getZ());
    float dotx = fromx.dot(atx) / (fromx.getX() - atx.getX());
    float doty = fromy.dot(aty) / (fromy.getY() - aty.getY());
    float dotz = fromz.dot(atz) / (fromz.getZ() - atz.getZ());
    float acosx = acos(dotx);
    float acosy = acos(doty);
    float acosz = acos(dotz);
    //std::cout << (fromx.getX() - atx.getX());
    return new Vector3(acosx, acosy, acosz);
}

Vector3* Camera::computeRayDirection(float x, float y,float near){
    float angle = tan(0.5 * this->_fov * M_PI/ 180.0);
    float vx = (2 * ((x + 0.5) * (1/this->_resx)) - 1) * angle * (this->_resx / this->_resy);
    float vy = (1 - 2 * ((y + 0.5) * (1/this->_resy))) * angle;
    Vector3* rayDirection  = new Vector3(vx, vy, near);
    // TODO: angles? quaternions? lolwut
    /*float rotx = atan2(this->_at->getY(), this->_at->getZ());
    float roty = atan2(this->_at->getX() * cos(rotx), this->_at->getZ());
    float rotz = atan2(cos(rotx), sin(rotx) * sin(roty));
    rayDirection->rotateX(rotx);
    rayDirection->rotateY(roty);
    rayDirection->rotateZ(rotz);*/
    //rayDirection->normalize();
    return rayDirection;
}