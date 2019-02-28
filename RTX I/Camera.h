#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <fstream>

#include "Vector3.h"

class Camera{
private:
    Vector3 *_from, *_at, *_up;
    float _resx, _resy, _angle, _hither;
public:

    Camera(float fromx, float fromy, float fromz,
             float atx, float aty, float atz,
             float upx, float upy, float upz,
             float resx, float resy,
             float angle, float hither);
    ~Camera();

    Vector3* getFrom();
    Vector3* getAt();
    Vector3* getUp();
    float getResX();
    float getResY();
    float getAngle();
    float getHither();

};

#endif