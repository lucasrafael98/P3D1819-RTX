#include "SceneObject.h"

SceneObject::SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx){
                    this->_mtl = new Material(r,g,b,diff,spec,shine,transm,refidx);
                 }

SceneObject::~SceneObject(){
    delete this->_mtl;
}

Material* SceneObject::getMaterial(){ return this->_mtl; }