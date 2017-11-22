#ifndef _UTILS_H
#define _UTILS_H

#include <string>

bool checkExtension(std::string path, std::string ext){
    std::string subStr = path.substr(path.find_last_of("."), path.length() - 1);
    
    if(subStr == ext)
        return true;
    return false;
}

#endif //_UTILS_H