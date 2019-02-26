#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <string>
#include <fstream>

class Camera{
private:
    int _resX;
    int _resY;
public:

    Camera(int resX, int resY);
    ~Camera();

    int getResX();
    int getResY();

};

#endif