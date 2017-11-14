#ifndef _PARSER_H
#define _PARSER_H

#include "Mesh.hpp"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

enum ObjectType{
    PRIMITIVE = 0,
    POLYGON = 1
};

class Parser{

    private:
        string filename;

    public:

        Parser(string fn):filename(fn){}

        list<Mesh*>* Read(){
            
            if(filename == "")
                return NULL;

            fstream file(filename.c_str(), ios::in);
            if(!file.is_open()){
                cerr << "Error opening " << filename.c_str() << endl;
                return NULL;
            }

            Mesh* mesh = new Mesh();
            list<Mesh*> *meshes = new list<Mesh*>();

            list<Vertex>* vertices = new list<Vertex>();
            Vertex vertice;
            float ignore;
            ObjectType ot;

            while(!file.eof()){
                
                file >> vertice.position.x >> vertice.position.y >> vertice.position.z;
                file >> ignore >> ignore >> ignore;

                //cout << "[" << iterations << "]( " << vertice.x << " , " << vertice.y << " , " << vertice.z << ") -- " << ignore << endl;

                vertices->push_back(vertice);
            }

            mesh->setVertices(vertices);
            meshes->push_back(mesh);

            return meshes;
        }

        ~Parser(){
        }
};

#endif //_PARSER_H