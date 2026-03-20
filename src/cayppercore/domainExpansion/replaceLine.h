#ifndef REPLACELINE_H
#define REPLACELINE_H
#include <fstream>

inline void replaceLinebyContent(std::ofstream& outFile,std::ifstream& inFile, std::string& strNew,std::string& strOld){
    if(!inFile||!outFile){
        throw std::runtime_error(std::string("ERROR: couldn't read or write to file"));
    }
    std::string Temp;
    while(inFile >> Temp){
        if(Temp==strOld){
            Temp=strNew;
        }
        outFile << Temp <<"\n";
    }
}
#endif // REPLACELINE_H
