#ifndef XFCECHANGER_H
#define XFCECHANGER_H
#include<filesystem>
#include<vector>
#include<string>
class xfcechanger
{
public:
    xfcechanger();
    int change(std::filesystem::path path,std::vector<std::string> monitors,std::string fillmode);
};

#endif // XFCECHANGER_H
