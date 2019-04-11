#ifndef BBOX_H
#define BBOX_H

#include "Vector3.h"
#include "Ray.h"

class BBox{
private:
    float _x0, _x1;
	float _y0, _y1;
	float _z0, _z1;
public:
    BBox();
    BBox(float x0, float x1, float y0, float y1, float z0, float z1);
    ~BBox(){}
	bool intersect(Ray ray, float& ti, Vector3 &tmin, Vector3 &tmax);
	bool inside(Vector3 point);
	Vector3 getNormal(const Vector3 &hitPoint);
	float getMinX() const;
	float getMinY() const;
	float getMinZ() const;
	float getMaxX() const;
	float getMaxY() const;
	float getMaxZ() const;
};

#endif