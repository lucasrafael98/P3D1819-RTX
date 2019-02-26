#include "scene.h"
#include "camera.h"

void Scene::loadNFF(std::string filename){
    std::ifstream nff(filename);
    
}

Scene::Scene(std::string filename){
    this->loadNFF(filename);
    this->_camera = new Camera(512,512);
}