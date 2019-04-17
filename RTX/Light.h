#ifndef LIGHT_H
#define LIGHT_H
#include "Vector3.h"
#include "Color.h"
#include <vector>

class Light {
private:
    std::vector<Vector3*> _altpos;
    Vector3* _pos;
    Color* _color;

public:
    Light(float x, float y, float z);
    Light(float x, float y, float z, float r, float g, float b);
    ~Light();

    Vector3* getPosition() const;
    Color* getColor() const;
    float attenuate(const float &r) const;
    void computeAreaLight(int samples, float area);
    Vector3* getAlternatePos(int i);
};

#endif