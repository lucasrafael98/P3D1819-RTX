#include "scene.h"

void Scene::loadNFF(std::string filename){
    std::ifstream nff(filename);
    
}

Scene::Scene(std::string filename){
    this->loadNFF(filename);
}