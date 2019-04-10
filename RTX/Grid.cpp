#include "Grid.h"

Grid::Grid(std::vector<SceneObject*> objects){
    // creating the grid's bbox
    // start with opposite values and change as required by object bboxes
    Vector3 min(std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max());
    Vector3 max(std::numeric_limits<float>::min(),
                std::numeric_limits<float>::min(),
                std::numeric_limits<float>::min());
    for(SceneObject* so: objects){
        if(so->getBBox()->getMinX() < min.getX())
            min.setX(so->getBBox()->getMinX() - EPSILON);
        if(so->getBBox()->getMinY() < min.getY())
            min.setY(so->getBBox()->getMinY() - EPSILON);
        if(so->getBBox()->getMinZ() < min.getZ())
            min.setZ(so->getBBox()->getMinZ() - EPSILON);

        if(so->getBBox()->getMaxX() < max.getX())
            max.setX(so->getBBox()->getMaxX() + EPSILON);
        if(so->getBBox()->getMaxY() < max.getY())
            max.setY(so->getBBox()->getMaxY() + EPSILON);
        if(so->getBBox()->getMaxZ() < max.getZ())
            max.setZ(so->getBBox()->getMaxZ() + EPSILON);
    }
    this->_bbox = new BBox(min.getX(), min.getY(), min.getZ(),
                            max.getX(), max.getY(), max.getZ());

    this->_dimensions = new Vector3(max - min);

    float s = std::pow((this->_dimensions->getX() * 
                this->_dimensions->getY() *
                this->_dimensions->getZ())/ objects.size(), 1.0/3.0);
    float ncelx = int((M * this->_dimensions->getX()) / s) + 1;
    float ncely = int((M * this->_dimensions->getY()) / s) + 1;
    float ncelz = int((M * this->_dimensions->getZ()) / s) + 1;
    this->_cellnum = new Vector3(ncelx, ncely, ncelz);
}

Grid::~Grid(){
    delete this->_bbox;
    delete this->_dimensions;
    delete this->_cellnum;
    for(Cell* cell: this->_cells){
        delete cell;
    }
}