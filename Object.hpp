#include "Mesh.hpp"

class Object{

    private:
        Mesh* mesh;
        int posX, posY, posZ;

    public:
        Object(Mesh* _mesh) : mesh(_mesh){}
        ~Object(){
            SafeDelete(mesh);
        }

        void update(){
            
        }

        void draw(){

        }
};