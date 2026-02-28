#ifndef JSSCRIPTLOADER_H
#define JSSCRIPTLOADER_H

#include <string>
#include <fstream>
#include <sstream>


std::string loadScript(const std::string& path){
    std::ifstream file(path);
    if(!file.is_open()){
        throw std::runtime_error("Failed to open .js script file-"+path);
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
#endif // JSSCRIPTLOADER_H
