#include "Polygon.h"

static int rayTriTests = 0;
static int rayTriIntersections = 0;

Polygon::Polygon(float r, float g, float b, float diff,
                 float spec, float shine, float transm,
                 float refidx, std::vector<Vector3*> verts):
                 SceneObject(r,g,b,diff,spec,shine,transm,refidx),
                  _verts(verts){
                        Vector3 sub21 = *(this->_verts.at(1)) - *(this->_verts.at(0));
                        Vector3 sub31 = *(this->_verts.at(2)) - *(this->_verts.at(0));
                        Vector3 normal = sub21.cross(sub31);
                        this->_normal = new Vector3(normal.getX(), normal.getY(), normal.getZ());
                        this->_normal->normalize();
                        this->setBBox(this->createBBox());
                  }
Polygon::Polygon(float r, float g, float b, float diff,
	float spec, float shine, float transm,
	float refidx, std::vector<Vector3*> verts, Vector3* normal) :
	SceneObject(r, g, b, diff, spec, shine, transm, refidx),
	_verts(verts), _normal(normal) {
	this->setBBox(this->createBBox());
}
Polygon::~Polygon(){
    for(Vector3* vec: this->_verts)
        delete vec;
    delete this->_normal;
}
Vector3* Polygon::getVertex(int i){ return this->_verts.at(i); }

Vector3 Polygon::getNormal(const Vector3 &hitPoint){ return *(this->_normal); }

bool Polygon::intersect(Ray ray, float& ti){
    rayTriTests++;
    Vector3 vertex0 = this->_verts.at(0);
    Vector3 vertex1 = this->_verts.at(1);  
    Vector3 vertex2 = this->_verts.at(2);
    float a,f,u,v;
    Vector3 edge1 = vertex1 - vertex0;
    Vector3 edge2 = vertex2 - vertex0;
    Vector3 h = ray.getDirection().cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // Ray and triangle are parallel.
    f = 1.0/a;
    Vector3 s = (ray.getOrigin()) - vertex0;
    u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0)
        return false;
    Vector3 q = s.cross(edge1);
    v = f * ray.getDirection().dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // Compute t, intersection point in the ray line.
    float t = f * edge2.dot(q);
    if (t > EPSILON){
        rayTriIntersections++;
        ti = t;
        return true;
    }
    else
        return false;
}

BBox* Polygon::createBBox(){
    return new BBox(std::min(this->_verts[0]->getX(), std::min(this->_verts[1]->getX(), this->_verts[2]->getX())) - EPSILON,
                    std::max(this->_verts[0]->getX(), std::max(this->_verts[1]->getX(), this->_verts[2]->getX())) + EPSILON,
                    std::min(this->_verts[0]->getY(), std::min(this->_verts[1]->getY(), this->_verts[2]->getY())) - EPSILON,
                    std::max(this->_verts[0]->getY(), std::max(this->_verts[1]->getY(), this->_verts[2]->getY())) + EPSILON,
                    std::min(this->_verts[0]->getZ(), std::min(this->_verts[1]->getZ(), this->_verts[2]->getZ())) - EPSILON,
                    std::max(this->_verts[0]->getZ(), std::max(this->_verts[1]->getZ(), this->_verts[2]->getZ())) + EPSILON);
}

void Polygon::printTotalIntersections(){std::cout << "Ray-Triangle Tests:\t\t" << rayTriTests << std::endl
                                                << "Ray-Triangle Intersections:\t" << rayTriIntersections << std::endl; }