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
        vector<float*> m_colors; //Vetor da posicao de cada vertex
        vector<glm::vec2> m_uvs; //Vetor de UVS
        vector<glm::vec3> m_normals;
        
        
        GLuint shaderProgram;
        GLuint MatrixMVPHandle;
        GLuint MatrixModelHandle;
        GLuint MatrixViewHandle;

        GLenum drawMode;
        float dT, angle;
        float moveSpeed, rotateSpeed;

        public:
            bool isSelected;
            vector<Vertex> m_vertices; //Vetor da estrutura Vertex
            vector<unsigned int> m_indices;

            GLuint vertexBuffer; //Buffer para os vertices
            GLuint normalBuffer; //Bufer para as normais
            GLuint colorBuffer; //Bufer para as normais
            GLuint uvBuffer;     //Buffer para as UV's
            GLuint elementBuffer; //Buffer para os indices
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scaleDim;

            Mesh(GLenum dm) : drawMode(dm), dT(0.0){}
            Mesh() : dT(0.0), angle(0.0), isSelected(false) {
                drawMode = GL_TRIANGLES;
                position = glm::vec3(0,0,0);
                rotation = glm::vec3(0,1,0);
                scaleDim = glm::vec3(1,1,1);
                moveSpeed = 0.25f;
                rotateSpeed = 0.125f;
            }

            ~Mesh(){
                glDeleteProgram(shaderProgram);
                glDeleteBuffers(1, &vertexBuffer);
                glDeleteBuffers(1, &normalBuffer);
                glDeleteBuffers(1, &elementBuffer);
                glDeleteBuffers(1, &colorBuffer);
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

                //Vertex color
                glEnableVertexAttribArray(2);
                glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
                glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

                glDrawElements(
                    drawMode,
                    m_indices.size(),
                    GL_UNSIGNED_INT,
                    (void*)0
                );


                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
                glDisableVertexAttribArray(2);
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
                m_colors.resize(m_vertices.size());
                int i = 0;
                for(vector<Vertex>::iterator v = m_vertices.begin(); v != m_vertices.end(); ++v, ++i){
                    m_vertex[i].x = (*v).position.x;
                    m_vertex[i].y = (*v).position.y;
                    m_vertex[i].z = (*v).position.z;

                    m_normals[i].x = (*v).normal.x;
                    m_normals[i].y = (*v).normal.y;
                    m_normals[i].z = (*v).normal.z;

                    m_colors[i] = (*v).color;
                }

                //Atributo 1 (POSICAO)
                glGenBuffers(1, &vertexBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(glm::vec3), &m_vertex[0], GL_STATIC_DRAW);

                //Atributo 2 (NORMAL)
                glGenBuffers(1, &normalBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
                glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

                //Atributo 3 (COLOR)
                glGenBuffers(1, &colorBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
                glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(float*), &m_colors[0], GL_STATIC_DRAW);

                //Index Buffer
                glGenBuffers(1, &elementBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);


                MatrixMVPHandle = glGetUniformLocation(shaderProgram, "MVP");
                MatrixModelHandle = glGetUniformLocation(shaderProgram, "Model");
                MatrixViewHandle = glGetUniformLocation(shaderProgram, "View");
            }

            void update(GLFWwindow* window){
                if(!isSelected)
                    return;

                if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                    position += glm::vec3(1,0,0) * moveSpeed;
                if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                    position -= glm::vec3(1,0,0) * moveSpeed;
                if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                    position += glm::vec3(0,0,1) * moveSpeed;
                if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                    position -= glm::vec3(0,0,1) * moveSpeed;
                if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
                    position += glm::vec3(0,1,0) * moveSpeed;
                if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
                    position -= glm::vec3(0,1,0) * moveSpeed;
                if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                    dT += rotateSpeed;
                if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
                    dT -= rotateSpeed;
                
                //dT += 0.025f;

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

            float deltaR = (bottomRadius - topRadius) / nStack;


            unsigned int numAnel = nStack + 1;

            for (unsigned int i = 0; i < numAnel; ++i) {

                float y = -0.5f*height + i * stackAltura;
                float r = bottomRadius + i * deltaR;

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

        void generateGrid(float width, float depth, unsigned int m, unsigned int n, MeshData::Mesh& modelo){
            unsigned int nVertex = m*n;
            unsigned int nFaces = (m-1)*(n-1) * 2;

            float widthHalf = width*0.5f;
            float depthHalf = depth*0.5f;

            float dx = width / (n-1);
            float dz = depth / (m-1);


            modelo.vertices()->resize(nVertex);
            MeshData::Vertex vert;
            for(unsigned int i = 0; i < m; ++i){
                float z = depthHalf - i * dz;

                for(unsigned int j = 0; j < n; ++j){
                    float x = -widthHalf + j * dx;
                    vert.position.x = x;
                    vert.position.y = 0.0f;
                    vert.position.z = z;
                    modelo.vertices()->at(i*n + j) = vert;
                }
            }


            modelo.indices()->resize(nFaces * 3);
            unsigned int k = 0;
            for(unsigned int i = 0; i < m-1; ++i){
                for(unsigned int j = 0; j < n-1; ++j){
                    modelo.indices()->at(k) = i*n + j;
                    modelo.indices()->at(k + 1) = i*n + j + 1;
                    modelo.indices()->at(k + 2) = (i + 1)*n + j;
                    modelo.indices()->at(k + 3) = (i + 1)*n + j;
                    modelo.indices()->at(k + 4) = i*n + j + 1;
                    modelo.indices()->at(k + 5) = (i + 1)*n + j + 1;
                    k += 6; // next quad
                }
            }
        }

        void generateCliff(float width, float depth, unsigned int m, unsigned int n, MeshData::Mesh& modelo){
            generateGrid(width,depth,m,n,modelo);

            std::vector<MeshData::Vertex> vertices(modelo.vertices()->size());

            for(unsigned int i = 0; i < modelo.vertices()->size(); ++i){
                glm::vec3 p = glm::vec3(modelo.vertices()->at(i).position.x,modelo.vertices()->at(i).position.y,modelo.vertices()->at(i).position.z);

                p.y = getHeight(p.x,p.z);

                vertices.at(i).position.x = p.x;
                vertices.at(i).position.y = p.y;
                vertices.at(i).position.z = p.z;

                glm::vec3 n = getHillNormal(p.x, p.z);

                vertices.at(i).normal.x = n.x;
                vertices.at(i).normal.y = n.y;
                vertices.at(i).normal.z = n.z;

                if (p.y < -10.0f){
                    float cor[4] = {1.0f, 0.96f, 0.62f, 1.0f}; 
			        vertices[i].color = cor;
                }
		        else if (p.y < 5.0f){
                    float cor[4] = {0.48f, 0.77f, 0.46f, 1.0f};
			        vertices[i].color = cor;
                }
		        else if (p.y < 12.0f){
			        float cor[4] = {0.1f, 0.48f, 0.19f, 1.0f};
                    vertices[i].color = cor;
                }
		        else if (p.y < 20.0f){
                    float cor[4] = {0.45f, 0.39f, 0.34f, 1.0f};
			        vertices[i].color = cor;
                }
		        else{
                    float cor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
			        vertices[i].color = cor; 
                }
            }
            modelo.setVertices(vertices);
        }


        void geosphere(float radius, unsigned int nSubDivisions, MeshData::Mesh& modelo){
            if (nSubDivisions < 5)
                nSubDivisions = 5;

            const float X = 0.525731f;
            const float Z = 0.850651f;

            glm::vec3 pos[12] = {
                vec3(-X, 0.0f, Z), vec3(X, 0.0f, Z),
                vec3(-X, 0.0f, -Z), vec3(X, 0.0f, -Z),
                vec3(0.0f, Z, X), vec3(0.0f, Z, -X),
                vec3(0.0f, -Z, X), vec3(0.0f, -Z, -X),
                vec3(Z, X, 0.0f), vec3(-Z, X, 0.0f),
                vec3(Z, -X, 0.0f), vec3(-Z, -X, 0.0f)
            };

            unsigned int k[60] = {
                1,4,0, 4,9,0, 4,5,9, 8,5,4, 1,8,4,
		        1,10,8, 10,3,8, 8,3,5, 3,2,5, 3,7,2,
		        3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		        10,1,6, 11,0,9, 2,11,9, 5,2,9, 11,2,7
            };


            modelo.vertices()->resize(12);
            modelo.indices()->resize(60);

            for(unsigned int i = 0; i < 12; ++i){
                modelo.vertices()->at(i).position.x = pos[i].x;
                modelo.vertices()->at(i).position.y = pos[i].y;
                modelo.vertices()->at(i).position.z = pos[i].z;
            }
            for(unsigned int i = 0; i < 60; ++i)
                modelo.indices()->at(i) = k[i];
            for(unsigned int i = 0; i < nSubDivisions; ++i)
                subdivide(modelo);

        	for (unsigned int i = 0; i < modelo.vertices()->size(); ++i) {
                vec3 pos = vec3(0.0f,0.0f,0.0f);
                pos.x = modelo.vertices()->at(i).position.x;
                pos.y = modelo.vertices()->at(i).position.y;
                pos.z = modelo.vertices()->at(i).position.z;

		        vec3 n = normalizeVector(pos);

        		vec3 p = radius * n;

                float color[3] = {
                    static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		            static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		            static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
                };

		        modelo.vertices()->at(i).position.x = p.x;
                modelo.vertices()->at(i).position.y = p.y;
                modelo.vertices()->at(i).position.z = p.z;
                modelo.vertices()->at(i).color = color;
	        }

        }


        void subdivide(MeshData::Mesh& modelo){
            MeshData::Mesh& copy = modelo;

            modelo.indices()->clear();
            modelo.vertices()->clear();

            unsigned int numTris = copy.indices()->size() / 3;
            for (unsigned int i = 0; i < numTris; ++i)
            {
                
                MeshData::Vertex v0 = copy.vertices()->at(copy.indices()->at(i * 3 + 0));
                MeshData::Vertex v1 = copy.vertices()->at(copy.indices()->at(i * 3 + 1));
                MeshData::Vertex v2 = copy.vertices()->at(copy.indices()->at(i * 3 + 2));


                MeshData::Vertex m0, m1, m2;

                m0.position.x = 0.5f*(v0.position.x + v1.position.x);
                m0.position.x = 0.5f*(v0.position.y + v1.position.y),
                m0.position.x = 0.5f*(v0.position.z + v1.position.z);

                m1.position.x = 0.5f*(v1.position.x + v2.position.x);
                m1.position.y = 0.5f*(v1.position.y + v2.position.y);
                m1.position.z = 0.5f*(v1.position.z + v2.position.z);

                m2.position.x = 0.5f*(v0.position.x + v2.position.x);
                m2.position.y = 0.5f*(v0.position.y + v2.position.y);
                m2.position.z = 0.5f*(v0.position.z + v2.position.z);

                modelo.vertices()->push_back(v0); // 0
                modelo.vertices()->push_back(v1); // 1
                modelo.vertices()->push_back(v2); // 2
                modelo.vertices()->push_back(m0); // 3
                modelo.vertices()->push_back(m1); // 4
                modelo.vertices()->push_back(m2); // 5

                modelo.indices()->push_back(i * 6 + 0);
                modelo.indices()->push_back(i * 6 + 3);
                modelo.indices()->push_back(i * 6 + 5);

                modelo.indices()->push_back(i * 6 + 3);
                modelo.indices()->push_back(i * 6 + 4);
                modelo.indices()->push_back(i * 6 + 5);

                modelo.indices()->push_back(i * 6 + 5);
                modelo.indices()->push_back(i * 6 + 4);
                modelo.indices()->push_back(i * 6 + 2);

                modelo.indices()->push_back(i * 6 + 3);
                modelo.indices()->push_back(i * 6 + 1);
                modelo.indices()->push_back(i * 6 + 4);
            }
        }


        float getHeight(float x, float z){return 0.3*(z*sinf(0.1f*x) + x*cosf(0.1f*z));}
        glm::vec3 getHillNormal(float x, float z){
            glm::vec3 p = glm::vec3(
                -0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		        1.0f,
		        -0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));
            
            return p;
        }
    };
}

#endif //_MESH_H