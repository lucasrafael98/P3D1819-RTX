#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "camera.h"
#include "color.h"
#include "vector3.h"

class Scene{
private:
    void loadNFF(std::string filename);
    Camera *_camera;
    Color* _bgColor;
public:

    Scene(std::string filename);
    ~Scene(){}

    Camera* getCamera();
    Color* getBGColor();

};

#endif