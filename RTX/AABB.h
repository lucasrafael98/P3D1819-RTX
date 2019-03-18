#ifndef AABB_H
#define AABB_H

#include "SceneObject.h"
#include "Vector3.h"
#include <algorithm>    // std::max min

class AABB :public SceneObject {
private:
	float _x0, _x1;
	float _y0, _y1;
	float _z0, _z1;

public:

	AABB(float r, float g, float b, float diff,
		float spec, float shine, float transm,
		float refidx, float x0, float x1, float y0, float y1,
		float z0, float z1);

	bool intersect(Ray ray, float& ti) override;
	Vector3 getNormal(const Vector3 &hitPoint) override;
};

#endif