#include "Scene.h"

std::vector<std::string> tokenizeLine(std::string line){
    std::vector<std::string> lin_proc;
    std::istringstream iss(line);
    for(std::string s; iss >> s;)
        lin_proc.push_back(s);
    return lin_proc;
}

void Scene::loadNFF(std::string filename){
    // TODO: check syntax errors in NFF? lol
    std::ifstream nff(filename);
    std::string line;
    // camera attrs
    Vector3 *fromVec, *atVec, *upVec;
    float camAngle, camHither, resX, resY;
    // current material to use on geometry
    float material[8];
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
            // positional light: xyz, optional rgb (1,1,1 white if not specified)
            if(lin_proc.size() == 4) 
                this->_lights.push_back(new Light(stof(lin_proc.at(1)), stof(lin_proc.at(2)), stof(lin_proc.at(3))));
            else if(lin_proc.size() == 7)
                this->_lights.push_back(new Light(stof(lin_proc.at(1)), stof(lin_proc.at(2)), stof(lin_proc.at(3)),
                                                    stof(lin_proc.at(4)), stof(lin_proc.at(5)), stof(lin_proc.at(6))));
        }
        else if(lin_proc.at(0) == std::string("f")){
            // change material
            for(int i = 1; i != 9; i++){
                material[i - 1] = stof(lin_proc.at(i));
            }
        }
        else if(lin_proc.at(0) == std::string("c")){
            // cone/cylinder
            std::getline(nff, line);
            std::vector<std::string> base = tokenizeLine(line);
            std::getline(nff, line);
            std::vector<std::string> apex = tokenizeLine(line);
            this->_objects.push_back(
                new Cone(material[0], material[1], material[2],
                             material[3], material[4], material[5], 
                             material[6], material[7],
                             stof(base.at(1)), stof(base.at(2)), 
                             stof(base.at(3)), stof(base.at(4)),
                             stof(apex.at(1)), stof(apex.at(2)), 
                             stof(apex.at(3)), stof(apex.at(4))));
        }
        else if(lin_proc.at(0) == std::string("s")){
            // sphere
            this->_objects.push_back(
                new Sphere(material[0], material[1], material[2],
                             material[3], material[4], material[5], 
                             material[6], material[7],
                             stof(lin_proc.at(1)), stof(lin_proc.at(2)), 
                             stof(lin_proc.at(3)), stof(lin_proc.at(4))));
        }
        else if(lin_proc.at(0) == std::string("p")){
            // TODO: polygon
        }
        else if(lin_proc.at(0) == std::string("pp")){
            // TODO: polygonal patch
        }
        else if(lin_proc.at(0) == std::string("pl")){
            // TODO: plane
        }
    }
}

Scene::Scene(std::string filename){
    this->loadNFF(filename);
    this->_camera = new Camera(256,256);
}

Scene::~Scene(){
    delete this->_camera;
    delete this->_bgColor;
    for(Light* l: this->_lights)
        delete l;
    for(SceneObject* so: this->_objects)
        delete so;
}

Camera* Scene::getCamera(){ return this->_camera; }
Color* Scene::getBGColor(){ return this->_bgColor; }
Light* Scene::getLight(int i){ return this->_lights.at(i); }