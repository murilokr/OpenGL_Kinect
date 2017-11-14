#ifndef _MESH_H
#define _MESH_H

#include "Renderer.hpp"
#include <math.h>
#include <fstream>
#include <vector>
#include <string>

#define GLEW_STATIC


namespace MeshData{

    typedef struct Transform{
        GLfloat x;
        GLfloat y;
        GLfloat z;
    }Pos;

    typedef struct Vertex{
        Transform position;
        Transform normal;
        float* color;

        Vertex(Transform pos, Transform nm, float* vC){
            position.x = pos.x;
            position.y = pos.y;
            position.z = pos.z;
            
            normal.x = nm.x;
            normal.y = nm.y;
            normal.z = nm.z;
            color = vC;
        }

        Vertex(){
            color = Color::White();
        }
    }Vertex;



    class Mesh{
        private:
        list<Vertex> *vertices;
        vector<Vertex> m_vertices;
        vector<unsigned int> m_indices;
        
        GLenum drawMode;
        float dT;

        public:
            GLfloat posX, posY, posZ;

            Mesh(list<Vertex> *vert, GLenum dm) : vertices(vert), drawMode(dm), dT(0.0){}
            Mesh() {
                this->vertices = new list<Vertex>();
                drawMode = GL_TRIANGLES;
                posX = 0.0f;
                posY = 0.0f;
                posZ = 0.0f;
            }

            ~Mesh(){
                SafeDelete(vertices);
            }

            list<Vertex>* returnVertices(){return this->vertices;}
            void setVertices(list<Vertex>* vert){this->vertices = vert;}

            GLenum getDrawMode(){return this->drawMode;}
            void setDrawMode(GLenum dm){this->drawMode = dm;}


            void draw(){

                static const GLfloat g_vertex_buffer_data[] = {
                    -1.0f, -1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f,
                    0.0f,  1.0f, 0.0f,
                };
                GLuint vertexbuffer;
                glGenBuffers(1, &vertexbuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                );
                // Draw the triangle !
                glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
                glDisableVertexAttribArray(0);

                
                /*
                GLuint vertexbuffer;
                glGenBuffers(1, &vertexbuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
                glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() - 1);
                glDisableVertexAttribArray(0);
                */
                /*
                glTranslatef(posX, posY, posZ);
                glBegin(drawMode);
                
                GLfloat vX, vY, vZ;
                GLfloat nX, nY, nZ;
                float r, g, b, a;

                for(list<Vertex>::iterator vertice = vertices->begin(); vertice != vertices->end(); ++vertice){
                    vX = (*vertice).position.x;
                    vY = (*vertice).position.y;
                    vZ = (*vertice).position.z;

                    nX = (*vertice).normal.x;
                    nY = (*vertice).normal.y;
                    nZ = (*vertice).normal.z;

                    glVertex3f(vX,vY,vZ);
                    glNormal3f(nX,nY,nZ);

                    Color::ColorToGL((*vertice).color);
                }*/
                //glEnd();
            }


            bool load(string modelname){
                if(modelname == "")
                    return false;
                
                //modelname.extension (skull.obj)
                fstream file(modelname.c_str(), ios::in);
                if(!file.is_open()){
                    cerr << "Error opening " << modelname << endl;
                    return false;
                }

                int nVertex, nIndex;
                string ignore;
                vector<unsigned int> vertexIndices, uvIndices, normalIndices;
                //vector<glm::vec2> uvs;

                file >> ignore >> nVertex;
                file >> ignore >> nIndex;
                file >> ignore >> ignore >> ignore >> ignore;


                m_vertices.resize(nVertex);
                for(unsigned int i = 0; i < nVertex; ++i){
                    file >> m_vertices[i].position.x >> m_vertices[i].position.y >> m_vertices[i].position.z;
                    file >> m_vertices[i].normal.x >> m_vertices[i].normal.y >> m_vertices[i].normal.z;
                }


                file >> ignore >> ignore >> ignore;


                m_indices.resize(nIndex);
                unsigned int tNumber = nIndex;
                for(unsigned int i = 0; i < tNumber / 3; ++i){
                    file >> m_indices[i * 3 + 0] >> m_indices[i * 3 + 1] >> m_indices[i * 3 + 2];
                }

                file.close();

                //build();
                return true;
            }

            void build(){
                if(m_vertices.size() == 0 || m_indices.size() == 0)
                    return;

                GLuint elementBuffer;
            }

            void update(){
                dT += 0.025f;
                float radius = 2.5f;

                posX = cos(dT) * radius;
                posY = sin(dT) * radius;
            }
    };

}

#endif //_MESH_H