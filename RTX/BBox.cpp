#include "BBox.h"

BBox::BBox(float x0, float x1, float y0, float y1,
	float z0, float z1) :
	_x0(x0), _x1(x1), _y0(y0), _y1(y1), _z0(z0), _z1(z1) {}
BBox::BBox():
	_x0(-1), _x1(1), _y0(-1), _y1(1), _z0(-1), _z1(1) {}

float BBox::getMinX() const{ return this->_x0; }
float BBox::getMinY() const{ return this->_y0; }
float BBox::getMinZ() const{ return this->_z0; }
float BBox::getMaxX() const{ return this->_x1; }
float BBox::getMaxY() const{ return this->_y1; }
float BBox::getMaxZ() const{ return this->_z1; }