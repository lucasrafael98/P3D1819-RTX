#include "scene.h"
#include "camera.h"

std::vector<std::string> tokenizeLine(std::string line){
    std::vector<std::string> lin_proc;
    std::istringstream iss(line);
    for(std::string s; iss >> s;)
        lin_proc.push_back(s);
    return lin_proc;
}

void Scene::loadNFF(std::string filename){
    std::ifstream nff(filename);
    std::string line;
    Vector3 *fromVec, *atVec, *upVec;
    float camAngle, camHither, resX, resY;
    while(std::getline(nff, line)){
        std::vector<std::string> lin_proc = tokenizeLine(line);
        if(lin_proc.at(0) == std::string("b")){
            // background color
            this->_bgColor = new Color(stof(lin_proc.at(1)), 
                                        stof(lin_proc.at(2)),
                                        stof(lin_proc.at(3)));
        }
        else if(lin_proc.at(0) == std::string("v")){
            // camera: add from, at, up, angle, hither, resolution
            std::getline(nff, line);
            std::vector<std::string> from = tokenizeLine(line);
            fromVec = new Vector3(stof(from.at(1)), stof(from.at(2)), stof(from.at(3)));
            std::getline(nff, line);
            std::vector<std::string> at = tokenizeLine(line);
            atVec = new Vector3(stof(at.at(1)), stof(at.at(2)), stof(at.at(3)));
            std::getline(nff, line);
            std::vector<std::string> up = tokenizeLine(line);
            upVec = new Vector3(stof(up.at(1)), stof(up.at(2)), stof(up.at(3)));
            std::getline(nff, line);
            std::vector<std::string> angle = tokenizeLine(line);
            camAngle = stof(angle.at(1));
            std::getline(nff, line);
            std::vector<std::string> hither = tokenizeLine(line);
            camHither = stof(hither.at(1));
            std::getline(nff, line);
            std::vector<std::string> res = tokenizeLine(line);
            resX = stof(res.at(1));
            resY = stof(res.at(2));
        }
        else if(lin_proc.at(0) == std::string("l")){
            // positional light
        }
        else if(lin_proc.at(0) == std::string("f")){
            // material properties
        }
        else if(lin_proc.at(0) == std::string("c")){
            // cone/cylinder
        }
        else if(lin_proc.at(0) == std::string("s")){
            // sphere
        }
        else if(lin_proc.at(0) == std::string("p")){
            // polygon
        }
        else if(lin_proc.at(0) == std::string("pp")){
            // polygonal patch
        }
    }
}

Scene::Scene(std::string filename){
    this->loadNFF(filename);
    this->_camera = new Camera(256,256);
}

Camera* Scene::getCamera(){ return this->_camera; }
Color* Scene::getBGColor(){ return this->_bgColor; }