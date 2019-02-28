#ifndef PLANE_H
#define PLANE_H

#include "Vector3.h"
#include "SceneObject.h"

class Plane: public SceneObject{
private:
    Vector3 *_pos1, *_pos2, *_pos3;
public:
    Plane();
    ~Plane();
    Vector3* getPos1();
    Vector3* getPos2();
    Vector3* getPos3();
};

#endif