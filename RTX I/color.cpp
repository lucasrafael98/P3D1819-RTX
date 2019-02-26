#include "color.h"

Color::Color(){
    this->_r = 0;
    this->_g = 0;
    this->_b = 0;
}
Color::Color(float r, float g, float b){
    this->_r = r;
    this->_g = g;
    this->_b = b;
}
Color::~Color(){}

float Color::getR(){return this->_r;}
float Color::getG(){return this->_g;}
float Color::getB(){return this->_b;}