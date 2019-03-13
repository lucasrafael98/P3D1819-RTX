#include "Material.h"

Material::Material(float r, float g, float b, float diff, float spec, float shine, float transm, float refidx):
    _diff(diff), _spec(spec), _shine(shine), _transm(transm), _refidx(refidx){
        this->_color = new Color(r,g,b);
}

Material::~Material(){ delete this->_color; }

Color* Material::getColor(){ return this->_color; }
float Material::getDiffuse(){ return this->_diff; }
float Material::getSpecular(){ return this->_spec; }
float Material::getShininess(){ return this->_shine; }
float Material::getTransmittance(){ return this->_transm; }
float Material::getRefractionIndex(){ return this->_refidx; }