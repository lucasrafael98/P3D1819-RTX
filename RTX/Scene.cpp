#include "Scene.h"

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
            std::cout << "=====BG COLOR=====\n" << this->_bgColor->getR() << "\t"
                                         << this->_bgColor->getB() << "\t"
                                         << this->_bgColor->getG() << "\t\n";
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
            this->_camera = new Camera(fromVec->getX(), fromVec->getY(), fromVec->getZ(),
                                        atVec->getX(), atVec->getY(), atVec->getZ(),
                                        upVec->getX(),upVec->getY(),upVec->getZ(),
                                        resX, resY, camAngle, camHither);
            std::cout << "=====CAMERA=====\nFROM\n" << this->_camera->getEye()->getX() << "\t"
                                             << this->_camera->getEye()->getY() << "\t"
                                             << this->_camera->getEye()->getZ() << "\t\n"
                        << "AT\n" << this->_camera->getAt()->getX() << "\t"
                                    << this->_camera->getAt()->getY() << "\t"
                                    << this->_camera->getAt()->getZ() << "\t\n"
                        << "UP\n" << this->_camera->getUp()->getX() << "\t"
                                    << this->_camera->getUp()->getY() << "\t"
                                    << this->_camera->getUp()->getZ() << "\t\n"
                        << "RESOLUTION\n" << this->_camera->getResX() << "by" << this->_camera->getResY() << "\n"
                        << "FOV\t" << this->_camera->getFOV() << "\n"
                        << "HITHER\t" << this->_camera->getNear() << "\n";


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
            std::cout << "=====MTL CHANGE=====\n" << "COLOR\t" << material[0] << "\t"
                                                     << material[1] << "\t"
                                                     << material[2] << "\n"
                                        << "DIFFUSE\t" << material[3] << "\n"
                                        << "SPECULAR\t" << material[4] << "\n"
                                        << "SHININESS\t" << material[5] << "\n"
                                        << "TRANSMITTANCE\t" << material[6] << "\n"
                                        << "REFRACTION INDEX\t" << material[7] <<"\n";
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
            std::vector<Vector3*> verts;
            for(int i = 0; i != stof(lin_proc.at(1)); i++){
                std::getline(nff, line);
                std::vector<std::string> vert = tokenizeLine(line);
                verts.push_back(new Vector3(stof(vert.at(0)), stof(vert.at(1)), stof(vert.at(2))));
            }
            this->_objects.push_back(
                new Polygon(material[0], material[1], material[2],
                             material[3], material[4], material[5], 
                             material[6], material[7], verts));
        }
        else if(lin_proc.at(0) == std::string("pp")){
            std::vector<Vector3*> verts;
            std::vector<Vector3*> norms;
            for(int i = 0; i != stof(lin_proc.at(1)); i++){
                std::getline(nff, line);
                std::vector<std::string> vert = tokenizeLine(line);
                verts.push_back(new Vector3(stof(vert.at(0)), stof(vert.at(1)), stof(vert.at(2))));
                norms.push_back(new Vector3(stof(vert.at(3)), stof(vert.at(4)), stof(vert.at(5))));
            }
            this->_objects.push_back(
                new PolyPlane(material[0], material[1], material[2],
                             material[3], material[4], material[5], 
                             material[6], material[7], verts, norms));
        }
        else if(lin_proc.at(0) == std::string("pl")){
            this->_objects.push_back(
                new Plane(material[0], material[1], material[2],
                             material[3], material[4], material[5], 
                             material[6], material[7],
                             stof(lin_proc.at(1)), stof(lin_proc.at(2)), 
                             stof(lin_proc.at(3)), stof(lin_proc.at(4)),
                             stof(lin_proc.at(5)), stof(lin_proc.at(6)),
                             stof(lin_proc.at(7)), stof(lin_proc.at(8)),
                             stof(lin_proc.at(9))));

        }
		else if (lin_proc.at(0) == std::string("aabb")) {
		// aabb
		this->_objects.push_back(
			new AABB(material[0], material[1], material[2],
				material[3], material[4], material[5],
				material[6], material[7],
				stof(lin_proc.at(1)), stof(lin_proc.at(2)),
				stof(lin_proc.at(3)), stof(lin_proc.at(4)),
				stof(lin_proc.at(5)), stof(lin_proc.at(6))));
		}
        else if(lin_proc.at(0) == std::string("#")) continue; // comment
    }
}

Scene::Scene(std::string filename){
    this->loadNFF(filename);
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
std::vector<Light*> Scene::getLights(){ return this->_lights; }
std::vector<SceneObject*> Scene::getObjectVector(){ return this->_objects; }