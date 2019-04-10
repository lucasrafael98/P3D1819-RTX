#include "SceneObject.h"

SceneObject::SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx){
                    this->_mtl = new Material(r,g,b,diff,spec,shine,transm,refidx);
                 }

SceneObject::~SceneObject(){
    delete this->_mtl;
    delete this->_bbox;
}

BBox* SceneObject::createBBox(){ return new BBox(); }

Material* SceneObject::getMaterial() const{ return this->_mtl; }
BBox* SceneObject::getBBox() const{ return this->_bbox; }
void SceneObject::setBBox(BBox* bbox){ this->_bbox = bbox; }