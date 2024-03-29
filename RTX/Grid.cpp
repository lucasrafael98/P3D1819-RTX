#include "Grid.h"
#include <mutex>

std::mutex mtx_rays;

int clamp(int n, int lower, int upper) {
	return std::max(lower, std::min(n, upper));
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
        if(so->getBBox()->getMinX() < min.getX()) min.setX(so->getBBox()->getMinX());
        if(so->getBBox()->getMinY() < min.getY()) min.setY(so->getBBox()->getMinY());
        if(so->getBBox()->getMinZ() < min.getZ()) min.setZ(so->getBBox()->getMinZ());

        if(so->getBBox()->getMaxX() > max.getX()) max.setX(so->getBBox()->getMaxX());
        if(so->getBBox()->getMaxY() > max.getY()) max.setY(so->getBBox()->getMaxY());
        if(so->getBBox()->getMaxZ() > max.getZ()) max.setZ(so->getBBox()->getMaxZ());
    }
    this->_bbox = new BBox(min.getX() - EPSILON, max.getX() + EPSILON, min.getY() - EPSILON,
                            max.getY() + EPSILON, min.getZ() - EPSILON, max.getZ() + EPSILON);
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
        int ixmin = clamp(int
                        ((objects.at(i)->getBBox()->getMinX() - this->_bbox->getMinX())
                            * (this->_cellnum->getX() / this->_dimensions->getX())),
                        0,int(this->_cellnum->getX() - 1));
        int iymin = clamp(int
                        ((objects.at(i)->getBBox()->getMinY() - this->_bbox->getMinY())
                            * (this->_cellnum->getY() / this->_dimensions->getY())),
                        0,int(this->_cellnum->getY() - 1));
        int izmin = clamp(int
                        ((objects.at(i)->getBBox()->getMinZ() - this->_bbox->getMinZ())
                            * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                        0,int(this->_cellnum->getZ() - 1));
        int ixmax = clamp(int
                        ((objects.at(i)->getBBox()->getMaxX() - this->_bbox->getMinX())
                            * (this->_cellnum->getX() / this->_dimensions->getX())),
                        0,int(this->_cellnum->getX() - 1));
        int iymax = clamp(int
                        ((objects.at(i)->getBBox()->getMaxY() - this->_bbox->getMinY())
                            * (this->_cellnum->getY() / this->_dimensions->getY())),
                        0,int(this->_cellnum->getY() - 1));
        int izmax = clamp(int
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

SceneObject* Grid::intersect(Ray ray, float &t0, bool mailbox){
    // If the ray doesn't even hit the grid BB, cease computation
    Vector3 tmin, tmax;
    if(!this->_bbox->intersect(ray, t0, tmin, tmax)) return nullptr;

    int ix, iy, iz;
    // Check if ray is inside the grid already
    if(this->_bbox->inside(ray.getOrigin())){
        ix = clamp(int
                    ((ray.getOrigin().getX() - this->_bbox->getMinX())
                    * (this->_cellnum->getX() / this->_dimensions->getX())),
                    0, int(this->_cellnum->getX() - 1));
        iy = clamp(int
                    ((ray.getOrigin().getY() - this->_bbox->getMinY())
                    * (this->_cellnum->getY() / this->_dimensions->getY())),
                    0, int(this->_cellnum->getY() - 1));
        iz = clamp(int
                    ((ray.getOrigin().getZ() - this->_bbox->getMinZ())
                    * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                    0, int(this->_cellnum->getZ() - 1));
    }
    // otherwise find the ray-grid intersection point 
    else{
        Vector3 p = ray.getOrigin() + ray.getDirection() * t0;
        ix = clamp(int
                    ((p.getX() - this->_bbox->getMinX())
                    * (this->_cellnum->getX() / this->_dimensions->getX())),
                    0, int(this->_cellnum->getX() - 1));
        iy = clamp(int
                    ((p.getY() - this->_bbox->getMinY())
                    * (this->_cellnum->getY() / this->_dimensions->getY())),
                    0, int(this->_cellnum->getY() - 1));
        iz = clamp(int
                    ((p.getZ() - this->_bbox->getMinZ())
                    * (this->_cellnum->getZ() / this->_dimensions->getZ())),
                    0, int(this->_cellnum->getZ() - 1));
    }
    Vector3 dt = (tmax - tmin) / *(this->_cellnum);
    float tnextx, tnexty, tnextz; 
    int stepx, stepy, stepz,
        stopx, stopy, stopz;
    computeTrajectory(dt.getX(), tnextx, stepx, this->_cellnum->getX(),
                    stopx, ray.getDirection().getX(), ix, tmin.getX());
    computeTrajectory(dt.getY(), tnexty, stepy, this->_cellnum->getY(),
                    stopy, ray.getDirection().getY(), iy, tmin.getY());
    computeTrajectory(dt.getZ(), tnextz, stepz, this->_cellnum->getZ(),
                    stopz, ray.getDirection().getZ(), iz, tmin.getZ());
    
    Vector3 tnext(tnextx, tnexty, tnextz),
            step(stepx, stepy, stepz),
            stop(stopx, stopy, stopz),
            i(ix, iy, iz);

    return traverseGrid(ray, i, tnext, step, stop, dt, t0, mailbox);
}

void Grid::computeTrajectory(float dt, float &tnext, int &step, float n,
                        int &stop, float dir, float i, float tmin){
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
                                Vector3 step, Vector3 stop, Vector3 dt, float &t0, bool mailbox){
    while(true){
        SceneObject* hit = nullptr;
        int index = i.getX() + 
                    (this->_cellnum->getX() * i.getY()) +
                    (this->_cellnum->getX() * this->_cellnum->getY() * i.getZ());
        std::vector<SceneObject*> objects = this->_cells.at(index)->getObjects();
        float tnear = INFINITY;
        for(SceneObject* so: objects){
            if(mailbox && so->getLastID() == ray.getID()) continue;
            if(so->intersect(ray, t0)){
                if(t0 < tnear){
                    tnear = t0;
                    hit = so;
                }
            }
            if(mailbox) so->setLastID(ray.getID());
        }
        if(tnext.getX() < tnext.getY() && tnext.getX() < tnext.getZ()){
            if(hit && tnear < tnext.getX()) return hit;
            else if(mailbox && hit) hit->setLastID(0);
            tnext.setX(tnext.getX() + dt.getX());
            i.setX(i.getX() + step.getX());
            if(i.getX() == stop.getX()) return nullptr;
        }
        else if(tnext.getY() < tnext.getZ()){
            if(hit && tnear < tnext.getY()) return hit;
            else if(mailbox && hit) hit->setLastID(0);
            tnext.setY(tnext.getY() + dt.getY());
            i.setY(i.getY() + step.getY());
            if(i.getY() == stop.getY()) return nullptr;
        }
        else{
            if(hit && tnear < tnext.getZ()) return hit;
            else if(mailbox && hit) hit->setLastID(0);
            tnext.setZ(tnext.getZ() + dt.getZ());
            i.setZ(i.getZ() + step.getZ());
            if(i.getZ() == stop.getZ()) return nullptr;
        }
    }
}