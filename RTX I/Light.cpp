#include "Light.h"

Light::Light(float x, float y, float z){
    this->_pos = new Vector3(x,y,z);
    this->_color = new Color(1,1,1);
}

Light::Light(float x, float y, float z, float r, float g, float b){
    this->_pos = new Vector3(x,y,z);
    this->_color = new Color(r,g,b);
}

Light::~Light(){
    delete this->_color;
    delete this->_pos;
}