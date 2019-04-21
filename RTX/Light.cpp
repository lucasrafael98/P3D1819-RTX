#include "Light.h"

Light::Light(float x, float y, float z){
    this->_pos = new Vector3(x,y,z);
    this->_color = new Color(1,1,1);
}

Light::Light(float x, float y, float z, float r, float g, float b){
    this->_pos = new Vector3(x,y,z);
    this->_color = new Color(r,g,b);
}

Light::~Light(){
    delete this->_color;
    delete this->_pos;
    for(Vector3* vec: this->_altpos)
        delete vec;
}

Vector3* Light::getPosition() const{ return this->_pos; }
Color* Light::getColor() const{ return this->_color; }
float Light::attenuate(const float &r) const { return 1.0; }
Vector3* Light::getAlternatePos(int i){ return this->_altpos.at(i); }

// Soft Shadows: compute SxS alternate positions in the specified light area, and then shuffle them.
void Light::computeAreaLight(int samples, float area){
    for (int p = 0; p < samples; p++) {
        for (int q = 0; q < samples; q++) {
            float randomFactor = ((float)rand() / (RAND_MAX)); //0 < random < 1
            this->_altpos.push_back(new Vector3(this->_pos->getX() + ((p + randomFactor) / samples * area),
                                    this->_pos->getY() + ((q + randomFactor) / samples * area),
                                    this->_pos->getZ()));
        }
    }
    for(int i = this->_altpos.size() - 1; i!=-1; i--){
        int j = (double)rand() / RAND_MAX * i;
        iter_swap(this->_altpos.begin() + i, this->_altpos.begin() + j);
    }
}