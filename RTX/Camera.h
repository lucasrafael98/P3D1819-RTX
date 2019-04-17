#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <fstream>

#include "Vector3.h"

class Camera{
private:
    Vector3 *_eye, *_at, *_up, *_xe, *_ye, *_ze;
    float _resx, _resy, _fov, _near, _w, _h, _df;
public:

    Camera(float fromx, float fromy, float fromz,
             float atx, float aty, float atz,
             float upx, float upy, float upz,
             float resx, float resy,
             float angle, float hither);
    ~Camera();

    Vector3* getEye();
    Vector3* getAt();
    Vector3* getUp();
	Vector3* getU();
	Vector3* getV();
	Vector3* getN();
    float getResX();
    float getResY();
    float getFOV();
    float getNear();
	float getDF();
	float getH();
	float getW();
    Vector3 computeRayDirection(float x, float y);
};

#endif