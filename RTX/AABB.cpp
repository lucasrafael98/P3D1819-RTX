#include "AABB.h"

AABB::AABB(float r, float g, float b, float diff,
	float spec, float shine, float transm,
	float refidx, float x0, float x1, float y0, float y1,
	float z0, float z1) :
	SceneObject(r, g, b, diff, spec, shine, transm, refidx), 
	_x0(x0), _x1(x1), _y0(y0), _y1(y1), _z0(z0), _z1(z1) {
}

float AABB::getMinX() const{ return this->_x0; }
float AABB::getMinY() const{ return this->_y0; }
float AABB::getMinZ() const{ return this->_z0; }
float AABB::getMaxX() const{ return this->_x1; }
float AABB::getMaxY() const{ return this->_y1; }
float AABB::getMaxZ() const{ return this->_z1; }

bool AABB::intersect(Ray ray, float& ti) {

	ray.getDirection().normalize();

	double ox = ray.getOrigin().getX(); double oy = ray.getOrigin().getY(); double oz = ray.getOrigin().getZ();
	double dx = ray.getDirection().getX(); double dy = ray.getDirection().getY(); double dz = ray.getDirection().getZ();

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (this->_x0 - ox) * a;
		tx_max = (this->_x1 - ox) * a;
	}
	else {
		tx_min = (this->_x1 - ox) * a;
		tx_max = (this->_x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (this->_y0 - oy) * b;
		ty_max = (this->_y1 - oy) * b;
	}
	else {
		ty_min = (this->_y1 - oy) * b;
		ty_max = (this->_y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (this->_z0 - oz) * c;
		tz_max = (this->_z1 - oz) * c;
	}
	else {
		tz_min = (this->_z1 - oz) * c;
		tz_max = (this->_z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if(tz_max < t1)
		t1 = tz_max;

	ti = (t0 < t1) ? t0 : t1;

	return (t0 < t1 && t1 > EPSILON);
}
Vector3 AABB::getNormal(const Vector3 &hitPoint) { 
	Vector3 c = Vector3(this->_x0 + this->_x1, this->_y0 + this->_y1, this->_z0 + this->_z1);
	c = c * 0.5;
	Vector3 point = hitPoint - c;
	Vector3 d = Vector3(this->_x0 - this->_x1, this->_y0 - this->_y1, this->_z0 - this->_z1);
	d = d * 0.5;
	float bias = 1.000001f;

	Vector3 normal = Vector3(float(int(point.getX() / fabsf(d.getX()) * bias)),
								float(int(point.getY() / fabsf(d.getY()) * bias)),
								float(int(point.getZ() / fabsf(d.getZ()) * bias)));
	normal.normalize();

	return normal;
}