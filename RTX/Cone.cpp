#include "Cone.h"

Cone::Cone(float r, float g, float b, float diff,
    float spec, float shine, float transm,
    float refidx, float basex, float basey, float basez, float baserad,
    float apexx, float apexy, float apexz, float apexrad):
    SceneObject(r,g,b,diff,spec,shine,transm,refidx), _apexRad(apexrad), _baseRad(baserad){
        _apexPos = new Vector3(apexx, apexy, apexz);
        _basePos = new Vector3(basex, basey, basez);
}
Cone::~Cone(){
    delete this->_basePos;
    delete this->_apexPos;
}

Vector3* Cone::getApexPosition(){ return this->_apexPos; }
Vector3* Cone::getBasePosition(){ return this->_basePos; }
float Cone::getApexRadius(){ return this->_apexRad; }
float Cone::getBaseRadius(){ return this->_baseRad; }

bool Cone::intersect(Ray ray, float& ti)
{
	if (this->getApexRadius() > 0) {
		Vector3 p0(ray.getOrigin()->getX() - this->_basePos->getX(), ray.getOrigin()->getY() - this->_basePos->getY(), ray.getOrigin()->getZ() - this->_basePos->getZ());

		double a = ray.getDirection()->getX()*ray.getDirection()->getX() + ray.getDirection()->getZ()*ray.getDirection()->getZ();
		double b = ray.getDirection()->getX()*p0.getX() + ray.getDirection()->getZ()*p0.getZ();
		double c = p0.getX()*p0.getX() + p0.getZ()*p0.getZ() - this->getBaseRadius() * this->getBaseRadius();

		double delta = b * b - a * c;

		double epsilon = 0.00000001;

		if (delta < EPSILON)
			return false;

		ti = (-b - sqrt(delta)) / a;

		if (ti <= EPSILON)
			return false;


		double y = p0.getY() + ti * ray.getDirection()->getY();

		if (y > this->height() + epsilon || y < -epsilon) {
			double dist;
			bool b1 = intersect_base(ray, this->getApexPosition(), dist);
			if (b1) ti = dist;
			bool b2 = intersect_base(ray, this->getBasePosition(), dist);
			if (b2 && dist > epsilon && ti >= dist)
				ti = dist;
			return b1 || b2;
		}

		return true;
	}
	else {
		this->calculateAngle();

		Vector3 p0(ray.getOrigin()->getX() - this->_basePos->getX(), ray.getOrigin()->getY() - this->_basePos->getY(), ray.getOrigin()->getZ() - this->_basePos->getZ());

		double a = cosAlphaSq * ray.getDirection()->getX()*ray.getDirection()->getX() + cosAlphaSq * ray.getDirection()->getZ()*ray.getDirection()->getZ() - sinAlphaSq * ray.getDirection()->getY()*ray.getDirection()->getY();
		double b = cosAlphaSq * ray.getDirection()->getX()*p0.getX() + cosAlphaSq * ray.getDirection()->getZ()*p0.getZ() - sinAlphaSq * ray.getDirection()->getY()*p0.getY();
		double c = cosAlphaSq * p0.getX()*p0.getX() + cosAlphaSq * p0.getZ()*p0.getZ() - sinAlphaSq * p0.getY()*p0.getY();

		double delta = b * b - a * c;

		if (delta < EPSILON)
			return false;

		ti = (-b - sqrt(delta)) / a;

		if (ti < EPSILON)
			return false;

		double y = p0.getY() + ti * ray.getDirection()->getY();

		if (y < -this->height() - EPSILON || y > EPSILON)
			return false;

		return true;
	}

}

Vector3 Cone::getNormal(const Vector3 &p)
{
	if (this->getApexRadius() > 0) {
		Vector3 n = Vector3(p.getX() - this->_basePos->getX(), 0, p.getZ() - this->_basePos->getZ());
		return n;
	}
	else {
		Vector3 c0(this->_basePos->getX(), p.getY(), this->_basePos->getZ());
		Vector3 c1(this->_basePos->getX(), this->_basePos->getY() + this->height(), this->_basePos->getZ());

		Vector3 v1 = p - this->getBasePosition();
		Vector3 v(v1.getX(),0,v1.getZ());
		v.normalize();

		Vector3 n(v.getX()*this->height() / this->getBaseRadius(), v.getZ()*this->height() / this->getBaseRadius(), this->getBaseRadius() / this->height());
		return n;
	}
}

bool Cone::intersect_base(Ray ray, const Vector3 &c, double& t)
{
	Vector3 normal = normal_in(c);
	Vector3 p0(ray.getOrigin()->getX() - this->_basePos->getX(), ray.getOrigin()->getY() - this->_basePos->getY(), ray.getOrigin()->getZ() - this->_basePos->getZ());
	double A = normal.getX();
	double B = normal.getY();
	double C = normal.getZ();
	double D = -(A*(c.getX() - this->_basePos->getX()) + B * (c.getY() - this->_basePos->getY()) + C * (c.getZ() - this->_basePos->getZ()));

	if (A*ray.getDirection()->getX() + B * ray.getDirection()->getY() + C * ray.getDirection()->getZ() == 0)
		return false;

	double dist = -(A*p0.getX() + B * p0.getY() + C * p0.getZ() + D) / (A*ray.getDirection()->getX() + B * ray.getDirection()->getY() + C * ray.getDirection()->getZ());

	if (dist < EPSILON)
		return false;

	Vector3 p(p0.getX() + dist * ray.getDirection()->getX(), p0.getY() + dist * ray.getDirection()->getY(), p0.getZ() + dist * ray.getDirection()->getZ());
	if (p.getX()*p.getX() + p.getZ()*p.getZ() - this->getBaseRadius() * this->getBaseRadius() > EPSILON)
		return false;

	t = dist;
	return true;
}

Vector3 Cone::normal_in(const Vector3 &p)
{
	// Point is on one of the bases
	if (p.getX()< this->_basePos->getX() + this->getBaseRadius() && p.getX()> this->_basePos->getX() - this->getBaseRadius() && p.getZ()< this->_basePos->getZ() + this->getBaseRadius() && p.getZ()> this->_basePos->getZ() - this->getBaseRadius())
	{
		if (p.getY() < this->_basePos->getY() + this->height() + EPSILON && p.getY()>this->_basePos->getY() + this->height() - EPSILON) {
			return Vector3(0, 1, 0);
		}
		if (p.getY() < this->_basePos->getY() + EPSILON && p.getY()> this->_basePos->getY() - EPSILON) {
			return Vector3(0, -1, 0);
		}
	}

	// Point is on lateral surface
	Vector3 c0(this->_basePos->getX(), this->_basePos->getY(), this->_basePos->getZ());
	Vector3 v = p - c0;
	v.normalize();
	return v;
}
