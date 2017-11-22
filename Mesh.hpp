#ifndef _MESH_H
#define _MESH_H

#include "Shader.hpp"
#include "Utils.hpp"
#include "Light.hpp"
#include <math.h>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#include <string.h>
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
        glm::vec2 uv;
        float* color;

        Vertex(Transform pos, Transform nm, glm::vec2 _uv, float* vC){
            position.x = pos.x;
            position.y = pos.y;
            position.z = pos.z;
            
            normal.x = nm.x;
            normal.y = nm.y;
            normal.z = nm.z;

            uv = _uv;
            color = vC;
        }

        Vertex(){
            color = Color::White;
        }
    }Vertex;



    class Mesh{
        private:
        vector<glm::vec3> m_vertex; //Vetor da posicao de cada vertex
        vector<glm::vec2> m_uvs; //Vetor de UVS
        vector<glm::vec3> m_normals;
        
        
        GLuint shaderProgram;
        GLuint MatrixMVPHandle;
        GLuint MatrixModelHandle;
        GLuint MatrixViewHandle;

        GLenum drawMode;
        float dT, angle;

        public:
            vector<Vertex> m_vertices; //Vetor da estrutura Vertex
            vector<unsigned int> m_indices;

            GLuint vertexBuffer; //Buffer para os vertices
            GLuint normalBuffer; //Bufer para as normais
            GLuint uvBuffer;     //Buffer para as UV's
            GLuint elementBuffer; //Buffer para os indices
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scaleDim;

            Mesh(GLenum dm) : drawMode(dm), dT(0.0){}
            Mesh() : dT(0.0), angle(0.0) {
                drawMode = GL_TRIANGLES;
                position = glm::vec3(0,0,0);
                rotation = glm::vec3(0,1,0);
                scaleDim = glm::vec3(1,1,1);
            }

            ~Mesh(){
                glDeleteProgram(shaderProgram);
                glDeleteBuffers(1, &vertexBuffer);
                glDeleteBuffers(1, &normalBuffer);
                glDeleteBuffers(1, &elementBuffer);
                glDeleteBuffers(1, &uvBuffer);
            }

            glm::mat4 modelMatrix(){
                return (glm::translate(glm::mat4(), position) * glm::rotate(glm::mat4(), dT, rotation) * glm::scale(glm::mat4(), scaleDim));
            }


            GLuint getShader(){return shaderProgram;}


            void loadShaders(const char* vertexShader, const char* fragShader){
                shaderProgram = Shader::LoadShaders(vertexShader, fragShader);
            }


            void draw(glm::mat4 mvp, glm::mat4 view){
                glUseProgram(shaderProgram);
                
                glm::mat4 model = modelMatrix();

                //Passar a matriz MVP para o shader
                glUniformMatrix4fv(MatrixMVPHandle, 1, GL_FALSE, &mvp[0][0]);
                glUniformMatrix4fv(MatrixModelHandle, 1, GL_FALSE, &model[0][0]);
                glUniformMatrix4fv(MatrixViewHandle, 1, GL_FALSE, &view[0][0]);

                //Vertex position
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

                //Vertex Normal
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
                glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

                glDrawElements(
                    drawMode,
                    m_indices.size(),
                    GL_UNSIGNED_INT,
                    (void*)0
                );


                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }


            bool load(string modelname, const char* vertexshader_path, const char* fragshader_path){
                if(checkExtension(modelname, ".obj"))
                    return loadOBJ(modelname, vertexshader_path, fragshader_path);
                else if(checkExtension(modelname, ".txt"))
                    return loadTXT(modelname, vertexshader_path, fragshader_path);
                return false;
            }


            bool loadOBJ(string modelname, const char* vertexshader_path, const char* fragshader_path){
                if(modelname == "")
                    return false;

                fstream file(modelname.c_str(), ios::in);
                if(!file.is_open()){
                    cerr << "Error opening " << modelname << endl;
                    return false;
                }



                return true;
            }


            bool loadTXT(string modelname, const char* vertexshader_path, const char* fragshader_path){
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
                for(unsigned int i = 0; i < triangleCount; ++i){
                    file >> m_indices[i * 3 + 0] >> m_indices[i * 3 + 1] >> m_indices[i * 3 + 2];
                }

                file.close();

                shaderProgram = Shader::LoadShaders(vertexshader_path, fragshader_path);

                build();
                return true;
            }

            void build(){
                if(m_vertices.size() == 0 || m_indices.size() == 0){
                    return;
                }

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
                //TODO

                //Index Buffer
                glGenBuffers(1, &elementBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);


                MatrixMVPHandle = glGetUniformLocation(shaderProgram, "MVP");
                MatrixModelHandle = glGetUniformLocation(shaderProgram, "Model");
                MatrixViewHandle = glGetUniformLocation(shaderProgram, "View");
            }

            void update(){
                dT += 0.025f;

                angle = cos(dT) * 2.5f;
            }

            GLenum getDrawMode(){return this->drawMode;}
            void setDrawMode(GLenum dm){this->drawMode = dm;}

            void setVertices(vector<Vertex> vert){this->m_vertices = vert;}
            vector<Vertex>* vertices(){return &this->m_vertices;}

            void setIndices(vector<unsigned int> index){this->m_indices = index;}
            vector<unsigned int>* indices(){return &this->m_indices;}
    };

}








namespace Geometry{
    class Geometry{
        public:

        void buildCylinderHatch(float radius, float height, float diff, unsigned int nSlices, MeshData::Mesh& modelo){
            unsigned int baseIndex = (unsigned int)modelo.vertices()->size();
            float y = diff*height;
            float dTheta = 2.0f*PI / nSlices;

            MeshData::Vertex vertex;
            for(unsigned int i = 0; i <= nSlices; ++i){
                float x = radius * cosf(i*dTheta);
                float z = radius * sinf(i*dTheta);

                float u = x / height + 0.5f;
                float v = z / height + 0.5f;

                vertex.position.x = x;
                vertex.position.y = y;
                vertex.position.z = z;
                modelo.vertices()->push_back(vertex);
            }

            vertex.position.x = 0.0f;
            vertex.position.z = 0.0f;
            modelo.vertices()->push_back(vertex);

            unsigned int centerIndex = (unsigned int)modelo.vertices()->size() - 1;
            for(unsigned int i = 0; i < nSlices; ++i){
                modelo.indices()->push_back(centerIndex);
                modelo.indices()->push_back(baseIndex + i + 1);
                modelo.indices()->push_back(baseIndex + i);
            }
        }

        void generateCylinder(float bottomRadius, float topRadius, float height, unsigned int nSlices, unsigned int nStack, MeshData::Mesh & modelo){
            if (topRadius > bottomRadius) {
                float aux = bottomRadius;
                bottomRadius = topRadius;
                topRadius = aux;
            }

            modelo.vertices()->clear();
            modelo.indices()->clear();

            float stackAltura = height / nStack;

            //Numero que determina o quanto eh preciso incrementar o raio, a cada vez q movemos para um level acima nos stacks.
            float deltaR = (bottomRadius - topRadius) / nStack;


            unsigned int numAnel = nStack + 1;

            for (unsigned int i = 0; i < numAnel; ++i) {

                float y = -0.5f*height + i * stackAltura;
                float r = bottomRadius + i * deltaR;


                //Criando os vertices no anel
                float dTheta = 2.0f*PI / nSlices;

                for (unsigned int j = 0; j <= nSlices; ++j) {

                    MeshData::Vertex vertex;
                    float c = cosf(j*dTheta);
                    float s = sinf(j*dTheta);

                    vertex.position.x = r*c;
                    vertex.position.y = y;
                    vertex.position.z = r*s;
                    ///vertex.Cor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

                    modelo.vertices()->push_back(vertex);
                }
            }
            cout << modelo.vertices()->size() << endl;



            //
            //	Criando os indices
            //

            unsigned int anelVertexN = nSlices + 1;

            for (unsigned int i = 0; i < nStack; ++i) {
                for (unsigned int j = 0; j < nSlices; ++j) {
                    modelo.indices()->push_back(i*anelVertexN + j);
                    modelo.indices()->push_back((i + 1)*anelVertexN + j);
                    modelo.indices()->push_back((i + 1)*anelVertexN + j + 1);
                    modelo.indices()->push_back(i*anelVertexN + j);	
                    modelo.indices()->push_back((i + 1)*anelVertexN + j + 1);
                    modelo.indices()->push_back(i*anelVertexN + j + 1);
                }
            }

            buildCylinderHatch(bottomRadius, height, -0.5f, nSlices, modelo);
            buildCylinderHatch(topRadius, height, 0.5f, nSlices, modelo);
        }
    };
}

#endif //_MESH_H