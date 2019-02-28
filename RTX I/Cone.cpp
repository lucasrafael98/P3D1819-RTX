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