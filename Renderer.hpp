#ifndef _RENDERER_H
#define _RENDERER_H

//*************************OPENGL*************************//
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glut.h>

//*************************C++*************************//
#include <iostream>
#include <list>
#include <math.h>

//*************************GLM*************************//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

//*************************LOCALS*************************//
#include "GlobalsDefines.hpp"
#include "Color.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

//*************************DEFINES*************************//
#define GLEW_STATIC
#define GL_GLEXT_PROTOTYPES

//*************************NAMESPACES*************************//
using namespace std;
using namespace MeshData;   
using namespace glm;



class Renderer{
    private:
        list<Mesh*>* mesh3D;
        list<Mesh*>* mesh2D;
        GLuint VertexArrayID;
        GLFWwindow* window;

        int winH, winW;

        glm::mat4 View; //Camera view (fazer a da classe ainda) PLACEHOLDER**************8
        

    public:

        Renderer() : winW(WINDOW_WIDTH), winH(WINDOW_HEIGHT){}
        ~Renderer(){
            SafeDelete(mesh3D);
            SafeDelete(mesh2D);

            glDeleteVertexArrays(1, &VertexArrayID);
        }


        void set3DMeshes(list<Mesh*>* meshes){mesh3D = meshes;}
        void set2DMeshes(list<Mesh*>* meshes){mesh2D = meshes;}
        list<Mesh*>* get3DMeshes(){return mesh3D;}
        list<Mesh*>* get2DMeshes(){return mesh2D;}

        void setClearColor(float* clearColor){
            glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        }

        bool initializeOpenGL(){
            if(!glfwInit())
                return false;

            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            
            window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
            if( window == NULL ){
                fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
                glfwTerminate();
                return -1;
            }

            glfwMakeContextCurrent(window);
            glewExperimental = true;
            if(glewInit() != GLEW_OK){
                fprintf(stderr, "Failed to initialize GLEW\n");
                return -1;
            }

            glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
            glfwSetWindowSizeCallback(window, resizeWindowCallback);

                // Dark blue background
            setClearColor(Color::DarkBlue);
            
            glGenVertexArrays(1, &VertexArrayID);
            glBindVertexArray(VertexArrayID);
            return true;
        }

        void loop(){
            do{
                this->update();
                this->draw();

            }while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

            glfwTerminate();
        }

        void update(){
            for(list<Mesh*>::iterator mesh = mesh3D->begin(); mesh != mesh3D->end(); ++mesh)
                (*mesh)->update();
        }


        void draw(){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            
            View = glm::lookAt(glm::vec3(4,3,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
            render3D();
            //render2D();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        void render3D(){
            if(mesh3D->size() == 0)
                return;

            glm::mat4 Projection = glm::perspective(glm::radians(90.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
            glm::mat4 Model;
            glm::mat4 mvp;

            for(list<Mesh*>::iterator mesh = mesh3D->begin(); mesh != mesh3D->end(); ++mesh){
                Model = (*mesh)->modelMatrix();

                mvp = Projection * View * Model;
                (*mesh)->draw(mvp);
            }
        }

        void render2D(){
            if(mesh2D->size() == 0)
                return;

            glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
            glm::mat4 Model = glm::mat4(1.0f);
            glm::mat4 mvp = Projection * View * Model;
        }



        static void resizeWindowCallback(GLFWwindow *window, int width, int height){
            WINDOW_WIDTH = width;
            WINDOW_HEIGHT = height;
            glViewport(0, 0, width, height);
        }
};

#endif //_RENDERER_H