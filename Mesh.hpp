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
        vector<Vertex> m_vertices; //Vetor da estrutura Vertex
        vector<glm::vec3> m_vertex; //Vetor da posicao de cada vertex
        vector<glm::vec3> m_normals;
        vector<unsigned int> m_indices;
        
        GLuint shaderProgram;
        GLuint MatrixMVPHandle;
        GLenum drawMode;
        float dT, angle;

        public:
            GLuint vertexBuffer; //Buffer para os vertices
            GLuint normalBuffer; //Bufer para as normais
            GLuint elementBuffer; //Buffer para os indices
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scaleDim;

            Mesh(list<Vertex> *vert, GLenum dm) : vertices(vert), drawMode(dm), dT(0.0){}
            Mesh() : dT(0.0), angle(0.0) {
                this->vertices = new list<Vertex>();
                drawMode = GL_TRIANGLES;
                position = glm::vec3(0,0,0);
                rotation = glm::vec3(0,1,0);
                scaleDim = glm::vec3(1,1,1);
            }

            ~Mesh(){
                SafeDelete(vertices);
                glDeleteProgram(shaderProgram);
                glDeleteBuffers(1, &vertexBuffer);
                glDeleteBuffers(1, &normalBuffer);
                glDeleteBuffers(1, &elementBuffer);
            }

            glm::mat4 modelMatrix(){
                return (glm::translate(glm::mat4(), position) * glm::rotate(glm::mat4(), dT, rotation) * glm::scale(glm::mat4(), scaleDim));
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

                file >> ignore >> nVertex;
                file >> ignore >> nIndex;
                file >> ignore >> ignore >> ignore >> ignore;


                m_vertices.resize(nVertex);
                for(unsigned int i = 0; i < nVertex; ++i){
                    file >> m_vertices[i].position.x >> m_vertices[i].position.y >> m_vertices[i].position.z;
                    file >> m_vertices[i].normal.x >> m_vertices[i].normal.y >> m_vertices[i].normal.z;
                }


                file >> ignore >> ignore >> ignore;

                unsigned int triangleCount = nIndex;
                nIndex *= 3;
                m_indices.resize(nIndex);
                for(unsigned int i = 0; i < triangleCount / 3; ++i){
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

                m_vertex.resize(m_vertices.size());
                m_normals.resize(m_vertices.size());
                int i = 0;
                for(vector<Vertex>::iterator v = m_vertices.begin(); v != m_vertices.end(); ++v, ++i){
                    m_vertex[i].x = (*v).position.x;
                    m_vertex[i].y = (*v).position.y;
                    m_vertex[i].z = (*v).position.z;

                    m_normals[i].x = (*v).normal.x;
                    m_normals[i].y = (*v).normal.y;
                    m_normals[i].z = (*v).normal.z;
                }

                //Atributo 1 (POSICAO)
                glGenBuffers(1, &vertexBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(glm::vec3), &m_vertex[0], GL_STATIC_DRAW);

                //Atributo 2 (NORMAL)
                glGenBuffers(1, &normalBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
                glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

                //Atributo 3 (UV)

                glGenBuffers(1, &elementBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

                MatrixMVPHandle = glGetUniformLocation(shaderProgram, "MVP");
            }

            void update(){
                dT += 0.025f;

                angle = cos(dT) * 2.5f;
            }

            GLenum getDrawMode(){return this->drawMode;}
            void setDrawMode(GLenum dm){this->drawMode = dm;}

            void setVertices(vector<Vertex> vert){this->m_vertices = vert;}
            void setIndices(vector<unsigned int> index){this->m_indices = index;}
    };

}








namespace Geometry{

    void generateCylinder(float bottomRadius, float topRadius, float height, unsigned int nSlices, unsigned int nStack, std::vector<MeshData::Vertex>& out_vertex, std::vector<unsigned int>& out_index){
        if (topRadius > bottomRadius) {
            float aux = bottomRadius;
            bottomRadius = topRadius;
            topRadius = aux;
        }

        out_vertex.clear();
        out_index.clear();

        float stackHeight = height / nStack;

        float deltaR = (bottomRadius - topRadius) / nStack;


        unsigned int nRings = nStack + 1;

        for (unsigned int i = 0; i < nRings; ++i) {

            float y = -0.5f*height + i * stackHeight;
            float r = bottomRadius + i * deltaR;


            //Criando os vertices no anel
            float dTheta = 2.0f*PI / nSlices;

            for (unsigned int j = 0; j <= nSlices; ++j) {

                MeshData::Vertex vertex;
                float c = cos(j*dTheta);
                float s = sin(j*dTheta);

                vertex.position.x = r*c;
                vertex.position.y = y;
                vertex.position.z = r*s;

                out_vertex.push_back(vertex);

            }
        }


        unsigned int anelVertexN = nSlices + 1;

        for (unsigned int i = 0; i < nStack; ++i) {
            for (unsigned int j = 0; j < nSlices; ++j) {
                out_index.push_back(i*anelVertexN + j);
                out_index.push_back((i + 1)*anelVertexN + j);
                out_index.push_back((i + 1)*anelVertexN + j + 1);
                out_index.push_back(i*anelVertexN + j);	
                out_index.push_back((i + 1)*anelVertexN + j + 1);
                out_index.push_back(i*anelVertexN + j + 1);
            }
        }

        //BuildCilindroTopo(bottomRadius, topRadius, altura, nCortes, nStack, modelo);
        //BuildCilindroBase(bottomRadius, topRadius, altura, nCortes, nStack, modelo);
    }


}

#endif //_MESH_H