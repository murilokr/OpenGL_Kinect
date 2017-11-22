#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Color.hpp"
#include "Shader.hpp"

using namespace glm;

class Light{

    protected:
        vec3 position;
        float intensity;
        float* color;
        GLuint LightID;
        GLuint LightIntensityID;
        GLuint LightColorID;

    public:
        Light(vec3 pos, float inten, float* col) : position(pos), intensity(inten), color(col){}

        virtual void Build(GLuint shaderProgram){
            LightID = glGetUniformLocation(shaderProgram, "luzPosition");
            //LightIntensityID = glGetUniformLocation(shaderProgram, "intensidadeLuz");
            //LightColorID = glGetUniformLocation(shaderProgram, "corLuz");
        }

        virtual void Render(){
            glUniform3f(LightID, position.x, position.y, position.z);
        }
};


class DirectionalLight : public Light{
    private:
        vec3 direction;
        GLuint LightDirID;

    public:
        DirectionalLight(vec3 pos, vec3 dir, float inten, float* col) : Light(pos, inten, col), direction(dir){
        }

        void Build(GLuint shaderProgram){
            LightID = glGetUniformLocation(shaderProgram, "luzPosition");
            LightDirID = glGetUniformLocation(shaderProgram, "luzDirection");
        }

        void Render(){
            glUniform3f(LightID, position.x, position.y, position.z);
            glUniform3f(LightDirID, direction.x, direction.y, direction.z);
        }
};

#endif //_LIGHT_H