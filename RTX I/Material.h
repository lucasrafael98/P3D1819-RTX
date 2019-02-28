#ifndef MATERIAL_H
#define MATERIAL_H

#include "Color.h"

class Material{
private:
    Color* _color;
    float _diff, _spec, _shine, _transm, _refidx;
public:
    Material(float r, float g, float b, float diff, float spec, float shine, float transm, float refidx);
    ~Material();

    Color* getColor();
    float getDiffuse();
    float getSpecular();
    float getShininess();
    float getTransmittance();
    float getRefractionIndex();

};

#endif