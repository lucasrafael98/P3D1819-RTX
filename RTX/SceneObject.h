#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Material.h"
#include "Vector3.h"
#include "Ray.h"
#include "BBox.h"

class SceneObject{
private:
    Material* _mtl;
    BBox* _bbox;
    unsigned int _lastID = 0;
    unsigned int _lastIntrs = 0;
public:
    SceneObject(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx);
    virtual ~SceneObject();
    Material* getMaterial() const;
    virtual bool intersect(Ray ray, float& ti){ return false; }
    virtual Vector3 getNormal(const Vector3 &hitPoint) { return Vector3(0,0,0); }
    virtual BBox* createBBox();
    BBox* getBBox() const;
    void setBBox(BBox* bbox);
    unsigned int getLastID();
    void setLastID(unsigned int newID);
    unsigned int getLastIntrs();
    void setLastIntrs(unsigned int newID);
};

#endif