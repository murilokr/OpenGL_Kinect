#ifndef _MESH_H
#define _MESH_H

#include "Shader.hpp"
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//#define GLEW_STATIC


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
            color = Color::White;
        }
    }Vertex;



    class Mesh{
        private:
        list<Vertex> *vertices;
        //vector<Vertex> m_vertices;
        vector<glm::vec3> m_vertices;
        vector<unsigned int> m_indices;
        
        GLuint shaderProgram;
        GLuint MatrixMVPHandle;
        GLenum drawMode;
        float dT;

        public:
            GLuint vertexBuffer; //Buffer para os vertices
            GLuint elementBuffer; //Buffer para os indices
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;

            Mesh(list<Vertex> *vert, GLenum dm) : vertices(vert), drawMode(dm), dT(0.0){}
            Mesh() {
                this->vertices = new list<Vertex>();
                drawMode = GL_TRIANGLES;
                position = glm::vec3(0,0,0);
                rotation = glm::vec3(0,0,0);
                scale = glm::vec3(0,0,0);
            }

            ~Mesh(){
                SafeDelete(vertices);
                glDeleteProgram(shaderProgram);
                glDeleteBuffers(1, &vertexBuffer);
                glDeleteBuffers(1, &elementBuffer);
            }

            glm::mat4 modelMatrix(){
                return glm::translate(glm::mat4(), position);
            }

            void draw(glm::mat4 mvp){
                glUseProgram(shaderProgram);
                
                //Passar a matriz MVP para o shader
                glUniformMatrix4fv(MatrixMVPHandle, 1, GL_FALSE, &mvp[0][0]);


                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

                glDrawElements(
                    drawMode,
                    m_indices.size(),
                    GL_UNSIGNED_INT,
                    (void*)0
                );


                glDisableVertexAttribArray(0);
            }


            bool load(string modelname, const char* vertexshader_path, const char* fragshader_path){
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

                shaderProgram = Shader::LoadShaders(vertexshader_path, fragshader_path);

                build();
                return true;
            }

            void build(){
                if(m_vertices.size() == 0 || m_indices.size() == 0)
                    return;

                glGenBuffers(1, &vertexBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

                glGenBuffers(1, &elementBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

                MatrixMVPHandle = glGetUniformLocation(shaderProgram, "MVP");
            }

            void update(){
            }

            GLenum getDrawMode(){return this->drawMode;}
            void setDrawMode(GLenum dm){this->drawMode = dm;}
    };

}

#endif //_MESH_H