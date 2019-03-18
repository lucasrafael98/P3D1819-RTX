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
	ray.getDirection()->normalize();

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
	return tNear < tFar;
}
Vector3 AABB::getNormal(const Vector3 &hitPoint) { 
	Vector3 c = new Vector3(this->_x0 + this->_x1, this->_y0 + this->_y1, this->_z0 + this->_z1);
	c = c * 0.5;
	Vector3 point = hitPoint - c;
	Vector3 d = new Vector3(this->_x0 - this->_x1, this->_y0 - this->_y1, this->_z0 - this->_z1);
	d = d * 0.5;
	float bias = 1.01f;

	if (hitPoint.getX() > this->_x0 && hitPoint.getX() < this->_x1) {
		if (hitPoint.getZ() == this->_z0)
			return new Vector3(0.0f, 0.0f, -1.0f);
		else if (hitPoint.getZ() == this->_z1)
			return new Vector3(0.0f, 0.0f, 1.0f);
	}	
	else if (hitPoint.getY() > this->_y0 && hitPoint.getY() < this->_y1) {
			if (hitPoint.getX() == this->_x0)
				return new Vector3(-1.0f, 0.0f, 0.0f);
			else if (hitPoint.getX() == this->_x1)
				return new Vector3(1.0f, 0.0f, 0.0f);
	}	
	else if (hitPoint.getZ() > this->_z0 && hitPoint.getZ() < this->_z1) {
			if (hitPoint.getY() == this->_y0)
				return new Vector3(0.0f, 1.0f, 0.0f);
			else if (hitPoint.getY() == this->_y1)
				return new Vector3(0.0f, -1.0f, 0.0f);
	}

	return new Vector3(0.0f, 1.0f, 0.0f);
}