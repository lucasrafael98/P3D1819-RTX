#include "Grid.h"

// simple clamp function (limit a number between min/max)
float clamp(float x, float min, float max){
    if(x < min) return min;
    else if(x > max) return max;
    return x; 
}

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

    // create 1D cell array
    for(int i = 0; i != ncelx * ncely * ncelz; i++)
        this->_cells.push_back(new Cell());

    // fill cells with objects
    for(int i = 0; i != objects.size(); i++){
        int ixmin = int(clamp(
                        ((objects[i]->getBBox()->getMinX() - this->_bbox->getMinX())
                            * (this->_dimensions->getX() / this->_dimensions->getX())),
                        0,this->_cellnum->getX() - 1));
        int iymin = int(clamp(
                        ((objects[i]->getBBox()->getMinY() - this->_bbox->getMinY())
                            * (this->_dimensions->getY() / this->_dimensions->getY())),
                        0,this->_cellnum->getY() - 1));
        int izmin = int(clamp(
                        ((objects[i]->getBBox()->getMinZ() - this->_bbox->getMinZ())
                            * (this->_dimensions->getZ() / this->_dimensions->getZ())),
                        0,this->_cellnum->getZ() - 1));
        int ixmax = int(clamp(
                        ((objects[i]->getBBox()->getMaxX() - this->_bbox->getMaxX())
                            * (this->_dimensions->getX() / this->_dimensions->getX())),
                        0,this->_cellnum->getX() - 1));
        int iymax = int(clamp(
                        ((objects[i]->getBBox()->getMaxY() - this->_bbox->getMaxY())
                            * (this->_dimensions->getY() / this->_dimensions->getY())),
                        0,this->_cellnum->getY() - 1));
        int izmax = int(clamp(
                        ((objects[i]->getBBox()->getMaxZ() - this->_bbox->getMaxZ())
                            * (this->_dimensions->getZ() / this->_dimensions->getZ())),
                        0,this->_cellnum->getZ() - 1));

        for (int ix = ixmin; ix < ixmax; i++){
            for (int iy = iymin; iy < iymax; i++){
                for (int iz = izmin; iz < izmax; i++){
                    int index = ix + 
                                (this->_cellnum->getX() * iy) +
                                (this->_cellnum->getX() * this->_cellnum->getY() * iz);
                    this->_cells.at(index)->addObject(objects[i]);
                }
            }
        }
    }

}

Grid::~Grid(){
    delete this->_bbox;
    delete this->_dimensions;
    delete this->_cellnum;
    for(Cell* cell: this->_cells){
        delete cell;
    }
}