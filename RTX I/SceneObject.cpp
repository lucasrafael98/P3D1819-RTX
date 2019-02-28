#include "SceneObject.h"

SceneObject::SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx, float x, float y, float z){
                    this->_mtl = new Material(r,g,b,diff,spec,shine,transm,refidx);
                    this->_pos = new Vector3(x,y,z);
                 }

SceneObject::~SceneObject(){
    delete this->_mtl;
    delete this->_pos;
}

Material* SceneObject::getMaterial(){ return this->_mtl; }
Vector3* SceneObject::getPosition(){ return this->_pos; }