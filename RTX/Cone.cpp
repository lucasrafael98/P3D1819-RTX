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

	float A = ray.getOrigin()->getX() - this->_basePos->getX();
	float B = ray.getOrigin()->getZ() - this->_basePos->getZ();
	float D = this->height() - ray.getOrigin()->getY() + this->_basePos->getY();

	float tan = (this->_baseRad / this->height()) * (this->_baseRad / this->height());

	float a = (ray.getDirection()->getX() * ray.getDirection()->getX()) + (ray.getDirection()->getZ() * ray.getDirection()->getZ()) - (tan*(ray.getDirection()->getY() * ray.getDirection()->getY()));
	float b = (2 * A*ray.getDirection()->getX()) + (2 * B*ray.getDirection()->getZ()) + (2 * tan*D*ray.getDirection()->getY());
	float c = (A*A) + (B*B) - (tan*(D*D));

	float delta = b * b - 4 * (a*c);
	if (fabsf(delta) < EPSILON) return false;
	if (delta < 0.0) return false;

	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);
	float t;

	if (t1 > t2) t = t2;
	else t = t1;

	float r = ray.getOrigin()->getY() + t * ray.getDirection()->getY();

	if ((r > this->_basePos->getY()) and (r < this->_basePos->getY() + this->height())) {
		ti = t;
		return true;
	}
	else return false;
}

Vector3 Cone::getNormal(const Vector3 &hitPoint)
{
	Vector3 distance = *(this->_apexPos) - *(this->_basePos);
	float height = distance.length();

	float r = sqrt((hitPoint.getX() - this->_basePos->getX())*(hitPoint.getX() - this->_basePos->getX()) + (hitPoint.getZ() - this->_basePos->getZ())*(hitPoint.getZ() - this->_basePos->getZ()));
	Vector3 n = Vector3(hitPoint.getX() - this->_basePos->getZ(), r*(this->_baseRad / height), hitPoint.getZ() - this->_basePos->getZ());
	n.normalize();
	return n;
}