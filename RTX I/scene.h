#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <fstream>
#include "camera.h"

class Scene{
private:
    void loadNFF(std::string filename);
    Camera *_camera;
public:

    Scene(std::string filename);
    ~Scene(){}

    Camera* getCamera(){
        return _camera;
    }

};

#endif