#include "Renderer.hpp"
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]){

    Renderer *renderer = new Renderer();
    if(!renderer->initializeOpenGL()){
        cerr << "Erro ao inicializar o OpenGL" << endl;
        return -1;
    }

    list<Mesh*>* meshes = new list<Mesh*>();
    Mesh* caveira = new Mesh();
    Mesh* carro = new Mesh();
    
    caveira->setDrawMode(GL_TRIANGLES);
    caveira->load("./Models/skullTrue.txt", "./Shaders/SimpleVertexShader.vertexshader", "./Shaders/SimpleFragmentShader.fragmentshader");
    caveira->scaleDim = glm::vec3(0.5f, 0.5f, 0.5f);
    caveira->position = glm::vec3(0, -2.0f, 0.0f);

    /*Geometry::Geometry geo;
    geo.generateCylinder(1.5f, 1.3f, 4.0f, 20, 20, *carro);
    carro->loadShaders("./Shaders/SimpleVertexShader.vertexshader", "./Shaders/SimpleFragmentShader.fragmentshader");
    carro->build();
    carro->scaleDim = glm::vec3(0.75f, 0.75f, 0.75f);
    carro->position = glm::vec3(0.0f, 2.0f, 0.0f);*/

    carro->setDrawMode(GL_TRIANGLES);
    carro->load("./Models/carTrue.txt", "./Shaders/SimpleVertexShader.vertexshader", "./Shaders/SimpleFragmentShader.fragmentshader");
    carro->scaleDim = glm::vec3(0.75f, 0.75f, 0.75f);
    carro->position = glm::vec3(0.0f, 3.0f, 0.0f);

    meshes->push_back(caveira);
    //meshes->push_back(carro);
    renderer->set3DMeshes(meshes);

    renderer->loop();
    
    SafeDelete(renderer);
    return 0;
}