#ifndef _CAMERA_H
#define _CAMERA_H

#include "Renderer.hpp"

class Camera{

    private:
        float x, y, z;
        float pitch, yaw, roll;

        float upX, upY, upZ;
        float rightX, rightY, rightZ;

    public:

        Camera(){}
        ~Camera(){}

        float setXYZ(float x, float y, float z){
        }

        float lookAt(float tX, float tY, float tZ){
            gluLookAt(x, y, z, tX, tY, tZ, upX, upY, upZ);
        }
};


#endif //_CAMERA_H