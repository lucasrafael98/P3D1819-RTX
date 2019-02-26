#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <string>
#include <fstream>

class Color{
private:
    float _r;
    float _g;
    float _b;
public:

    Color();
    Color(float r, float g, float b);
    ~Color();

    float getR();
    float getG();
    float getB();

};

#endif