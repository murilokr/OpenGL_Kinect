#ifndef _COLOR_H
#define _COLOR_H

#include <list>
#include <iostream>

using namespace std;

namespace Color{

    float* Color(float r, float g, float b, float a){
        float *color = new float(4);

        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
        return color;
    }

    float White[] = {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    };

    float Black[] = {
        0.0f,
        0.0f,
        0.0f,
        1.0f
    };

    float Red[] = {
        1.0f,
        0.0f,
        0.0f,
        1.0f
    };

    float Green[] = {
        0.0f,
        1.0f,
        0.0f,
        1.0f
    };

    float Blue[] = {
        0.0f,
        0.0f,
        1.0f,
        1.0f
    };

    float DarkBlue[] = {
        0.0f,
        0.2f,
        0.4f,
        1.0f
    };

    float Cyan[] = {
        0.0f,
        1.0f,
        1.0f,
        1.0f
    };

    float Yellow[] = {
        1.0f,
        1.0f,
        0.0f,
        1.0f
    };

    float Pink[] = {
        1.0f,
        0.0f,
        1.0f,
        1.0f
    };

    void ColorToGL(float* color){
        float r = color[0];
        float g = color[1];
        float b = color[2];
        float a = color[3];

        glColor4f(r, g, b, a);
    }
}

#endif //_COLOR_H