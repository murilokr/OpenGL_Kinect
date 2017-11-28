#ifndef _UTILS_H
#define _UTILS_H

#include <string>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

glm::vec3 normalizeVector(const glm::vec3 v){
    float mag = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return glm::vec3(v.x/mag, v.y/mag, v.z/mag);
}

bool checkExtension(std::string path, std::string ext){
    std::string subStr = path.substr(path.find_last_of("."), path.length() - 1);
    
    if(subStr == ext)
        return true;
    return false;
}

#endif //_UTILS_H