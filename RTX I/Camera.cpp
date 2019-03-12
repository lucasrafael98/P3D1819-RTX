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
            this->_eye->normalize();
            this->_at->normalize();
            this->_up->normalize();
            Vector3 ze = (*(this->_eye) - *(this->_at)) / (*(this->_eye) - *(this->_at)).length();
            ze.normalize();
            this->_ze = new Vector3(ze.getX(), ze.getY(), ze.getZ());
            Vector3 xe = this->_up->cross(ze) / this->_up->cross(ze).length();
            xe.normalize();
            this->_xe = new Vector3(xe.getX(), xe.getY(), xe.getZ());
            Vector3 ye = ze.cross(xe);
            ye.normalize();
            this->_ye = new Vector3(ye.getX(), ye.getY(), ye.getZ());
            this->_h = 2 * ((*(this->_eye) - *(this->_at)).length()) * tan(0.5 * (this->_fov * M_PI / 180) );
	        this->_w = this->_resx / this->_resy * this->_h;
        }
Camera::~Camera(){}

Vector3* Camera::getEye(){ return this->_eye;}
Vector3* Camera::getAt(){ return this->_at;}
Vector3* Camera::getUp(){ return this->_up;}
float Camera::getResX(){ return this->_resx;}
float Camera::getResY(){ return this->_resy;}
float Camera::getFOV(){ return this->_fov;}
float Camera::getNear(){ return this->_near;}

Vector3* Camera::computeRayDirection(float x, float y){
    Vector3* dir = new Vector3(*(this->_ze) * -((*(this->_eye) - *(this->_at)).length()) 
                        + *(this->_ye) * this->_h * ((y / this->_resy) - 0.5)
                        + *(this->_xe) * this->_w * ((x / this->_resx) - 0.5));
    dir->normalize();
    return dir;
}