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

    float* White(){
        float* color = new float(4);

        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
        return color;
    }

    float* Black(){
        float* color = new float(4);

        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        return color;
    }

    float* Red(){
        float* color = new float(4);

        color[0] = 1.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        return color;
    }

    float* Green(){
        float* color = new float(4);

        color[0] = 0.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        return color;
    }

    float* Blue(){
        float* color = new float(4);

        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
        return color;
    }

    float* DarkBlue(){
        float* color = new float(4);

        color[0] = 0.0f;
        color[1] = 0.2f;
        color[2] = 0.4f;
        color[3] = 1.0f;
        return color;
    }

    float* Cyan(){
        float* color = new float(4);

        color[0] = 0.0f;
        color[1] = 1.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
        return color;
    }

    float* Yellow(){
        float* color = new float(4);

        color[0] = 1.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        return color;
    }

    float* Pink(){
        float* color = new float(4);

        color[0] = 1.0f;
        color[1] = 0.0f;
        color[2] = 1.0f;
        color[3] = 1.0f;
        return color;
    }

    void ColorToGL(float* color){
        float r = color[0];
        float g = color[1];
        float b = color[2];
        float a = color[3];

        glColor4f(r, g, b, a);
    }
}

#endif //_COLOR_H