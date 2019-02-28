#ifndef CONE_H
#define CONE_H

#include "SceneObject.h"
#include "Vector3.h"

// This class describes a cone OR a ****cylinder****!

class Cone: public SceneObject{
private:
    Vector3 *_apexPos, *_basePos;
    float _apexRad, _baseRad;
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
};

#endif