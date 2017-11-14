#ifndef _RENDERER_H
#define _RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <list>

#include "GlobalsDefines.hpp"
#include "Color.hpp"
#include "Mesh.hpp"
#include <math.h>

#define GLEW_STATIC
#define GL_GLEXT_PROTOTYPES

using namespace std;
using namespace MeshData;   



class Renderer{
    private:
        list<Mesh*>* mesh3D;
        list<Mesh*>* mesh2D;
        //static Renderer* currentInstance;

        void drawCallback();   
        void setupDrawCallback();
        int winH, winW;
        

    public:

        Renderer() : winW(WINDOW_WIDTH), winH(WINDOW_HEIGHT){}
        ~Renderer(){
            SafeDelete(mesh3D);
            SafeDelete(mesh2D);
        }


        void set3DMeshes(list<Mesh*>* meshes){mesh3D = meshes;}
        void set2DMeshes(list<Mesh*>* meshes){mesh2D = meshes;}
        list<Mesh*>* get3DMeshes(){return mesh3D;}
        list<Mesh*>* get2DMeshes(){return mesh2D;}

        void setClearColor(float* clearColor){
            glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        }

        bool initializeOpenGL(int argc, char* argv[]){
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
            glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
            glutInitWindowPosition(450, 300);
            glutCreateWindow("OpenGL 3D Engine");

            if(glewInit() != GLEW_OK)
                return false;
            this->setupDrawCallback();
            setClearColor(Color::DarkBlue());
            return true;
        }

        void loop(){
            glutMainLoop();
        }


        void resize(int w, int h){
            winH = h;
            winW = w;
        }

        void update(){
            for(list<Mesh*>::iterator mesh = mesh3D->begin(); mesh != mesh3D->end(); ++mesh)
                (*mesh)->update();
                
            glutPostRedisplay();
        }


        void draw(){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);
            //glEnable(GL_LIGHTING);
            

            render3D();

            glDisable(GL_DEPTH_TEST);
            //glDisable(GL_LIGHTING);

            render2D();

            glutSwapBuffers();
        }

        void render3D(){
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45, (winH==0)?(1):((float)winW/winH), 1, 100);
            
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            gluLookAt(0,0,-15,0,0,0,0,1,0);
            for(list<Mesh*>::iterator mesh = mesh3D->begin(); mesh != mesh3D->end(); ++mesh)
                (*mesh)->draw();

            glPopMatrix();
        }

        void render2D(){
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
            
            
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glPopMatrix();
        }

};

Renderer* currentInstance;

extern "C"
void drawCallback(){
    currentInstance->draw();
}

extern "C"
void resizeCallback(int w, int h){
    currentInstance->resize(w, h);
}

extern "C"
void updateCallback(){
    currentInstance->update();
}

void Renderer::setupDrawCallback(){
    ::currentInstance = this;
    ::glutDisplayFunc(::drawCallback);
    ::glutReshapeFunc(::resizeCallback);
    ::glutIdleFunc(::updateCallback);
}

#endif //_RENDERER_H