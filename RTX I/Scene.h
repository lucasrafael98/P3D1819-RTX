#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "Camera.h"
#include "Color.h"
#include "Vector3.h"
#include "Light.h"

class Scene{
private:
    void loadNFF(std::string filename);
    Camera *_camera;
    Color* _bgColor;
    std::vector<Light*> _lights;
public:

    Scene(std::string filename);
    ~Scene();

    Camera* getCamera();
    Color* getBGColor();
    Light* getLight(int i);

};

#endif