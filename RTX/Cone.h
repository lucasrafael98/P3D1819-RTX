#ifndef CONE_H
#define CONE_H

#include "SceneObject.h"
#include "Vector3.h"

// This class describes a cone OR a ****cylinder****!

class Cone: public SceneObject{
private:
    Vector3 *_apexPos, *_basePos;
    float _apexRad, _baseRad, cosAlphaSq, sinAlphaSq;
public:
    Cone(float r, float g, float b, float diff,
        float spec, float shine, float transm,
        float refidx, float basex, float basey, float basez, float baserad,
        float apexx, float apexy, float apexz, float apexrad);
    ~Cone();

    Vector3* getApexPosition();
    Vector3* getBasePosition();
    float getApexRadius();
    float getBaseRadius();
	float height() {
		Vector3 distance = *(this->_apexPos) - *(this->_basePos);
		return distance.length();
	}

	void calculateAngle()
	{
		Vector3 p1(this->_basePos->getX() + this->getBaseRadius(), this->_basePos->getY(), this->_basePos->getZ());
		Vector3 p2(this->_basePos->getX(), this->_basePos->getY() + this->height(), this->_basePos->getZ());
		double d = (p2 - p1).length();
		cosAlphaSq = this->height() / d;
		sinAlphaSq = this->getBaseRadius() / d;
	}

	bool intersect(Ray ray, float& ti) override;
	bool intersect_base(Ray ray, const Vector3 &c, double& t);
	Vector3 getNormal(const Vector3 &hitPoint) override;
	Vector3 normal_in(const Vector3 &hitPoint);


};

#endif