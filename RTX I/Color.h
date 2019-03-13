#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <string>
#include <fstream>
#include "Vector3.h"

class Color{
private:
    float _r;
    float _g;
    float _b;
public:

    Color();
    Color(float r, float g, float b);
    ~Color();

    float getR() const;
    float getG() const;
    float getB() const;
    Color operator * (float f) const { return Color(this->_r * f, this->_g * f, this->_b * f); }
    Color operator * (Vector3 f) const { return Color(this->_r * f.getX(), this->_g * f.getY(), this->_b * f.getZ()); }
    Color operator * (Color c) const { return Color(this->_r * c.getR(), this->_g * c.getG(), this->_b * c.getB()); }
    Color operator + (Color c) const { return Color(this->_r + c.getR(), this->_g + c.getG(), this->_b + c.getB()); } 
	Color operator - (Color c) const { return Color(this->_r - c.getR(), this->_g - c.getG(), this->_b - c.getB()); }

};

#endif