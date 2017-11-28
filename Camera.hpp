#ifndef _CAMERA_H
#define _CAMERA_H

#include <GLFW/glfw3.h>
#include "Renderer.hpp"

class Camera{

    private:
        glm::vec3 position;
        float horizontalAngle, verticalAngle, initialFoV, speed, mouseSpeed;

    public:

        Camera() : position(0.0, 0.0, 5), horizontalAngle(3.14), verticalAngle(0), initialFoV(45.0), speed(3.0), mouseSpeed(0.005){}
        ~Camera(){}

        void update(GLFWwindow* window){
            double xpos, ypos; //mouse position
            glfwGetCursorPos(window, &xpos, &ypos);

            glfwSetCursorPos(window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
        }

        glm::mat4 calculateView(){

        }
};


#endif //_CAMERA_H