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
#include "SceneObject.h"
#include "Sphere.h"
#include "Cone.h"
#include "Plane.h"
#include "Polygon.h"
#include "PolyPlane.h"
#include "AABB.h"
#include "Grid.h"

class Scene{
private:
    void loadNFF(std::string filename);
    Camera *_camera;
    Color* _bgColor;
    std::vector<Light*> _lights;
    std::vector<SceneObject*> _objects;
    std::vector<Plane*> _planes;
    Grid* _grid;
public:

    Scene(std::string filename);
    ~Scene();

    Camera* getCamera();
    Color* getBGColor();
    Grid* getGrid();
    Light* getLight(int i);
    std::vector<Light*> getLights();
    std::vector<Plane*> getPlanes();
    std::vector<SceneObject*> getObjectVector();

};

#endif