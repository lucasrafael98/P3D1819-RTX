#include "AABB.h"

AABB::AABB(float r, float g, float b, float diff,
	float spec, float shine, float transm,
	float refidx, float x0, float x1, float y0, float y1,
	float z0, float z1) :
	SceneObject(r, g, b, diff, spec, shine, transm, refidx), 
	_x0(x0), _x1(x1), _y0(y0), _y1(y1), _z0(z0), _z1(z1) {
	std::cout << x0 << x1 << y0 << y1 << z0 << z1 << std::endl;
}

bool AABB::intersect(Ray ray, float& ti) {
	/*ray.getDirection()->normalize();

	Vector3 tMin = new Vector3((this->_x0 - ray.getOrigin()->getX()) / ray.getDirection()->getX(),
		(this->_y0 - ray.getOrigin()->getY()) / ray.getDirection()->getY(),
		(this->_z0 - ray.getOrigin()->getZ()) / ray.getDirection()->getZ());

	Vector3 tMax = new Vector3((this->_x1 - ray.getOrigin()->getX()) / ray.getDirection()->getX(),
		(this->_y1 - ray.getOrigin()->getY()) / ray.getDirection()->getY(),
		(this->_z1 - ray.getOrigin()->getZ()) / ray.getDirection()->getZ());

	Vector3 t1 = new Vector3(std::min(tMin.getX(), tMax.getX()),
		std::min(tMin.getY(), tMax.getY()),
		std::min(tMin.getZ(), tMax.getZ()));
	Vector3 t2 = new Vector3(std::max(tMin.getX(), tMax.getX()),
		std::max(tMin.getY(), tMax.getY()),
		std::max(tMin.getZ(), tMax.getZ()));

	float tNear = std::max(std::max(t1.getX(), t1.getY()), t1.getZ());
	float tFar = std::min(std::min(t2.getX(), t2.getY()), t2.getZ());

	ti = tNear;
	return tNear < tFar;*///THIS DOES NOT WORK FOR SOME REASON???

	ray.getDirection()->normalize();

	double ox = ray.getOrigin()->getX(); double oy = ray.getOrigin()->getY(); double oz = ray.getOrigin()->getZ();
	double dx = ray.getDirection()->getX(); double dy = ray.getDirection()->getY(); double dz = ray.getDirection()->getZ();

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
	/* //n funciona direito pq Z afinal é altura e não Y, e acho que X cresce para o outro lado... é preciso arranjar as coordenadas como deve ser
	if (hitPoint.getZ() - this->_z0 < 0.01f) {
		//edge normals? do they even exist?
		if (hitPoint.getY() - this->_y0 < 0.01f) return new Vector3(0.0f, -1.0f, -1.0f);
		if (hitPoint.getY() - this->_y1 < 0.01f) return new Vector3(0.0f, 1.0f, -1.0f);
		if (hitPoint.getX() - this->_x0 < 0.01f) return new Vector3(-1.0f, 0.0f, -1.0f);
		if (hitPoint.getX() - this->_x1 < 0.01f) return new Vector3(1.0f, 0.0f, -1.0f);
		return new Vector3(0.0f, 0.0f, -1.0f);
	}
	else if (hitPoint.getZ() - this->_z1 < 0.01f) {
		//edge normals? do they even exist?
		if (hitPoint.getY() - this->_y0 < 0.01f) return new Vector3(0.0f, -1.0f, 1.0f);
		if (hitPoint.getY() - this->_y1 < 0.01f) return new Vector3(0.0f, 1.0f, 1.0f);
		if (hitPoint.getX() - this->_x0 < 0.01f) return new Vector3(-1.0f, 0.0f, 1.0f);
		if (hitPoint.getX() - this->_x1 < 0.01f) return new Vector3(1.0f, 0.0f, 1.0f);
		return new Vector3(0.0f, 0.0f, 1.0f);
	}

	if (hitPoint.getX() - this->_x0 < 0.01f) {
		//edge normals? do they even exist?
		if (hitPoint.getY() - this->_y0 < 0.01f) return new Vector3(-1.0f, -1.0f, 0.0f);
		if (hitPoint.getY() - this->_y1 < 0.01f) return new Vector3(-1.0f, 1.0f, 0.0f);
		if (hitPoint.getZ() - this->_z0 < 0.01f) return new Vector3(-1.0f, 0.0f, -1.0f);
		if (hitPoint.getZ() - this->_z1 < 0.01f) return new Vector3(-1.0f, 0.0f, 1.0f);
		return new Vector3(-1.0f, 0.0f, 0.0f);
	}
	else if (hitPoint.getX() - this->_x1 < 0.01f) {
		//edge normals? do they even exist?
		if (hitPoint.getY() - this->_y0 < 0.01f) return new Vector3(1.0f, -1.0f, 0.0f);
		if (hitPoint.getY() - this->_y1 < 0.01f) return new Vector3(1.0f, 1.0f, 0.0f);
		if (hitPoint.getZ() - this->_z0 < 0.01f) return new Vector3(1.0f, 0.0f, -1.0f);
		if (hitPoint.getZ() - this->_z1 < 0.01f) return new Vector3(1.0f, 0.0f, 1.0f);
		return new Vector3(1.0f, 0.0f, 0.0f);
	}

	if (hitPoint.getY() - this->_y0 < 0.01f) {
		//edge normals? do they even exist?
		if (hitPoint.getX() - this->_x0 < 0.01f) return new Vector3(-1.0f, -1.0f, 0.0f);
		if (hitPoint.getX() - this->_x1 < 0.01f) return new Vector3(1.0f, -1.0f, 0.0f);
		if (hitPoint.getZ() - this->_z0 < 0.01f) return new Vector3(0.0f, -1.0f, -1.0f);
		if (hitPoint.getZ() - this->_z1 < 0.01f) return new Vector3(0.0f, -1.0f, 1.0f);
		return new Vector3(0.0f, -1.0f, 0.0f);
	}
	else if (hitPoint.getY() - this->_y1 < 0.01f) {
		//edge normals? do they even exist?
		if (hitPoint.getX() - this->_x0 < 0.01f) return new Vector3(-1.0f, 1.0f, 0.0f);
		if (hitPoint.getX() - this->_x1 < 0.01f) return new Vector3(1.0f, 1.0f, 0.0f);
		if (hitPoint.getZ() - this->_z0 < 0.01f) return new Vector3(0.0f, 1.0f, -1.0f);
		if (hitPoint.getZ() - this->_z1 < 0.01f) return new Vector3(0.0f, 1.0f, 1.0f);
		return new Vector3(0.0f, 1.0f, 0.0f);
	}

	return new Vector3(0.0f, 1.0f, 0.0f);*/

	Vector3 c = new Vector3(this->_x0 + this->_x1, this->_y0 + this->_y1, this->_z0 + this->_z1);
	c = c * 0.5;
	Vector3 point = hitPoint - c;
	Vector3 d = new Vector3(this->_x0 - this->_x1, this->_y0 - this->_y1, this->_z0 - this->_z1);
	d = d * 0.5;
	float bias = 1.000001f;

	Vector3 normal = new Vector3(float(int((point.getX() / abs(d.getX()) * bias))),
		float(int((point.getY() / abs(d.getY()) * bias))),
		float(int((point.getZ() / abs(d.getZ()) * bias))));
	normal.normalize();

	return normal;
}