#include "Renderer.hpp"
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]){
    Renderer *renderer = new Renderer();
    if(renderer->initializeOpenGL()){
        cerr << "Erro ao inicializar o OpenGL" << endl;
        return -1;
    }

    list<Mesh*>* meshes = new list<Mesh*>();
    Mesh* caveira = new Mesh();
    caveira->setDrawMode(GL_TRIANGLES);
    caveira->load("./Models/carTrue.txt", "./Shaders/SimpleVertexShader.vertexshader", "./Shaders/SimpleFragmentShader.fragmentshader");

    meshes->push_back(caveira);
    renderer->set3DMeshes(meshes);

    renderer->loop();
    
    SafeDelete(renderer);
    return 0;
}