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
        if(so->getBBox()->getMinX() < min.getX()) min.setX(so->getBBox()->getMinX() - EPSILON);
        if(so->getBBox()->getMinY() < min.getY()) min.setY(so->getBBox()->getMinY() - EPSILON);
        if(so->getBBox()->getMinZ() < min.getZ()) min.setZ(so->getBBox()->getMinZ() - EPSILON);

        if(so->getBBox()->getMaxX() > max.getX()) max.setX(so->getBBox()->getMaxX() + EPSILON);
        if(so->getBBox()->getMaxY() > max.getY()) max.setY(so->getBBox()->getMaxY() + EPSILON);
        if(so->getBBox()->getMaxZ() > max.getZ()) max.setZ(so->getBBox()->getMaxZ() + EPSILON);
    }
    this->_bbox = new BBox(min.getX(), max.getX(), min.getY(),
                            max.getY(), min.getZ(), max.getZ());
    this->_dimensions = new Vector3(max - min);

    float s = std::pow((this->_dimensions->getX() * 
                this->_dimensions->getY() *
                this->_dimensions->getZ()) / objects.size(), 1.0f/3.0f);
    int ncelx = int(M * this->_dimensions->getX() / s) + 1;
    int ncely = int(M * this->_dimensions->getY() / s) + 1;
    int ncelz = int(M * this->_dimensions->getZ() / s) + 1;
    this->_cellnum = new Vector3(ncelx, ncely, ncelz);

    // create 1D cell array
    for(int i = 0; i < (ncelx * ncely * ncelz); i++)
        this->_cells.push_back(new Cell());

    // fill cells with objects
    for(unsigned int i = 0; i < objects.size(); i++){
        int ixmin = std::clamp(int
                        ((objects.at(i)->getBBox()->getMinX() - this->_bbox->getMinX())
                            * (this->_cellnum->getX() / this->_dimensions->getX())),
                        0,int(this->_cellnum->getX() - 1));
        int iymin = std::clamp(int
                        ((objects.at(i)->getBBox()->getMinY() - this->_bbox->getMinY())
                            * (this->_cellnum->getY() / this->_dimensions->getY())),
                        0,int(this->_cellnum->getY() - 1));
        int izmin = std::clamp(int
                        ((objects.at(i)->getBBox()->getMinZ() - this->_bbox->getMinZ())
                            * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                        0,int(this->_cellnum->getZ() - 1));
        int ixmax = std::clamp(int
                        ((objects.at(i)->getBBox()->getMaxX() - this->_bbox->getMinX())
                            * (this->_cellnum->getX() / this->_dimensions->getX())),
                        0,int(this->_cellnum->getX() - 1));
        int iymax = std::clamp(int
                        ((objects.at(i)->getBBox()->getMaxY() - this->_bbox->getMinY())
                            * (this->_cellnum->getY() / this->_dimensions->getY())),
                        0,int(this->_cellnum->getY() - 1));
        int izmax = std::clamp(int
                        ((objects.at(i)->getBBox()->getMaxZ() - this->_bbox->getMinZ())
                            * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                        0,int(this->_cellnum->getZ() - 1));
        for (int iz = izmin; iz <= izmax; iz++){
            for (int iy = iymin; iy <= iymax; iy++){
                for (int ix = ixmin; ix <= ixmax; ix++){
                    int index = ix + 
                            (this->_cellnum->getX() * iy) +
                            (this->_cellnum->getX() * this->_cellnum->getY() * iz);
                    this->_cells.at(index)->addObject(objects.at(i));
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

SceneObject* Grid::intersect(Ray ray, float &t0){
    // If the ray doesn't even hit the grid BB, cease computation
    Vector3 tmin, tmax;
    if(!this->_bbox->intersect(ray, t0, tmin, tmax)) return nullptr;

    int ix, iy, iz;
    // Check if ray is inside the grid already
    if(this->_bbox->inside(ray.getOrigin())){
        ix = std::clamp(int
                    ((ray.getOrigin().getX() - this->_bbox->getMinX())
                    * (this->_cellnum->getX() / this->_dimensions->getX())),
                    0, int(this->_cellnum->getX() - 1));
        iy = std::clamp(int
                    ((ray.getOrigin().getY() - this->_bbox->getMinY())
                    * (this->_cellnum->getY() / this->_dimensions->getY())),
                    0, int(this->_cellnum->getY() - 1));
        iz = std::clamp(int
                    ((ray.getOrigin().getZ() - this->_bbox->getMinZ())
                    * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                    0, int(this->_cellnum->getZ() - 1));
    }
    // otherwise find the ray-grid intersection point 
    else{
        Vector3 p = ray.getOrigin() + ray.getDirection() * t0;
        ix = std::clamp(int
                    ((p.getX() - this->_bbox->getMinX())
                    * (this->_cellnum->getX() / this->_dimensions->getX())),
                    0, int(this->_cellnum->getX() - 1));
        iy = std::clamp(int
                    ((p.getY() - this->_bbox->getMinY())
                    * (this->_cellnum->getY() / this->_dimensions->getY())),
                    0, int(this->_cellnum->getY() - 1));
        iz = std::clamp(int
                    ((p.getZ() - this->_bbox->getMinZ())
                    * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                    0, int(this->_cellnum->getZ() - 1));
    }
    Vector3 dt = (tmax - tmin) / *(this->_cellnum);
    float tnextx, tnexty, tnextz, 
            stepx, stepy, stepz,
            stopx, stopy, stopz;
    computeTnext(dt.getX(), tnextx, stepx, this->_cellnum->getX(),
                    stopx, ray.getDirection().getX(), ix, tmin.getX());
    computeTnext(dt.getY(), tnexty, stepy, this->_cellnum->getY(),
                    stopy, ray.getDirection().getY(), iy, tmin.getY());
    computeTnext(dt.getZ(), tnextz, stepz, this->_cellnum->getZ(),
                    stopz, ray.getDirection().getZ(), iz, tmin.getZ());
    
    Vector3 tnext(tnextx, tnexty, tnextz),
            step(stepx, stepy, stepz),
            stop(stopx, stopy, stopz),
            i(ix, iy, iz);

    return traverseGrid(ray, i, tnext, step, stop, dt, t0);
}

void Grid::computeTnext(float dt, float &tnext, float &step, float n,
                        float &stop, float dir, float i, float tmin){
    if(dir > 0){
        tnext = tmin + (i + 1) * dt;
        step = 1;
        stop = n; 
    }
    else{
        tnext = tmin + (n - i) * dt;
        step = -1;
        stop = -1; 
    }
    if(dir == 0){
        tnext = INFINITY;
        step = -1;
        stop = -1; 
    }
}

SceneObject* Grid::traverseGrid(Ray ray, Vector3 i, Vector3 tnext, 
                                Vector3 step, Vector3 stop, Vector3 dt, float &t0){
    while(true){
        SceneObject* hit = nullptr;
        int index = i.getX() + 
                    (this->_cellnum->getX() * i.getY()) +
                    (this->_cellnum->getX() * this->_cellnum->getY() * i.getZ());
        std::vector<SceneObject*> objects = this->_cells.at(index)->getObjects();
        float tnear = INFINITY;
        for(unsigned int i = 0; i < objects.size(); i++){
            if(objects.at(i)->intersect(ray, t0)){
                if(t0 < tnear){
                    tnear = t0;
                    hit = objects.at(i);
                }
            }
        }
        if(tnext.getX() < tnext.getY() && tnext.getX() < tnext.getZ()){
            if(hit && tnear < tnext.getX()) return hit;
            tnext.setX(tnext.getX() + dt.getX());
            i.setX(i.getX() + step.getX());
            if(i.getX() == stop.getX()) return nullptr;
        }
        else if(tnext.getY() < tnext.getZ()){
            if(hit && tnear < tnext.getY()) return hit;
            tnext.setY(tnext.getY() + dt.getY());
            i.setY(i.getY() + step.getY());
            if(i.getY() == stop.getY()) return nullptr;
        }
        else{
            if(hit && tnear < tnext.getZ()) return hit;
            tnext.setZ(tnext.getZ() + dt.getZ());
            i.setZ(i.getZ() + step.getZ());
            if(i.getZ() == stop.getZ()) return nullptr;
        }
    }
}