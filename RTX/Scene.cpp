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
            delete fromVec;
            delete atVec;
            delete upVec;
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
                             stof(base.at(0)), stof(base.at(1)), 
                             stof(base.at(2)), stof(base.at(3)),
                             stof(apex.at(0)), stof(apex.at(1)), 
                             stof(apex.at(2)), stof(apex.at(3))));
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
		else if (lin_proc.at(0) == std::string("ply")) {
			std::vector<Vector3*> raw_verts;
			std::vector<Vector3*> raw_normals;
			std::vector<int> face_indices;
			std::ifstream ply(std::string("NFF/" + lin_proc.at(1)));
			std::string line_ply;
			bool hended = false;
			while (std::getline(ply, line_ply)) {
				std::vector<std::string> lin_proc_ply = tokenizeLine(line_ply);
				if (hended) {
					if (lin_proc_ply.size() == 6) {
						raw_verts.push_back(new Vector3(stof(lin_proc_ply.at(0)), stof(lin_proc_ply.at(1)), stof(lin_proc_ply.at(2))));
						raw_normals.push_back(new Vector3(stof(lin_proc_ply.at(3)), stof(lin_proc_ply.at(4)), stof(lin_proc_ply.at(5))));
					}
					else if (lin_proc_ply.size() == 4) {
						face_indices.push_back(stoi(lin_proc_ply.at(1)));
						face_indices.push_back(stoi(lin_proc_ply.at(2)));
						face_indices.push_back(stoi(lin_proc_ply.at(3)));
					}
					else {
						std::cout << "WTF NOOO" << std::endl;
					}
				}
				else {
					if (lin_proc_ply.at(0) == std::string("end_header")) {
						hended = true;
					}
				}
			}
			std::cout << raw_verts.size() << raw_normals.size() << std::endl;
			std::vector<std::vector<Vector3*>> verts;
			std::vector<Vector3*> normals;
			for (unsigned int i = 0; i != face_indices.size(); i+=3) {
				std::vector<Vector3*> temp;
				temp.push_back(raw_verts.at(face_indices.at(i)));
				temp.push_back(raw_verts.at(face_indices.at(i+1)));
				temp.push_back(raw_verts.at(face_indices.at(i+2)));
				verts.push_back(temp);
				normals.push_back(raw_normals.at(face_indices.at(i+2)));
			}
			std::cout << "OI: " << verts.size() << std::endl;
			for (unsigned int i = 0; i != verts.size(); i++) {
				this->_objects.push_back(
					new Polygon(material[0], material[1], material[2],
						material[3], material[4], material[5],
						material[6], material[7], verts.at(i),normals.at(i)));
			}
		}
        else if(lin_proc.at(0) == std::string("pl")){
            this->_planes.push_back(
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
    this->_grid = new Grid(this->_objects);
}

Scene::Scene(std::string filename){
    this->loadNFF(filename);
}

Scene::~Scene(){
    delete this->_camera;
    delete this->_bgColor;
    delete this->_grid;
    for(Light* l: this->_lights)
        delete l;
    for(SceneObject* so: this->_objects)
        delete so;
}

Camera* Scene::getCamera(){ return this->_camera; }
Color* Scene::getBGColor(){ return this->_bgColor; }
Grid* Scene::getGrid(){ return this->_grid; }
Light* Scene::getLight(int i){ return this->_lights.at(i); }
std::vector<Light*> Scene::getLights(){ return this->_lights; }
std::vector<Plane*> Scene::getPlanes(){ return this->_planes; }
std::vector<SceneObject*> Scene::getObjectVector(){ return this->_objects; }