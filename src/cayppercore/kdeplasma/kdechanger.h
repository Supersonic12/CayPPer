#ifndef KDECHANGER_H
#define KDECHANGER_H
#include <vector>
#include <string>
#include <filesystem>
class kdechanger
{
public:
    kdechanger();
    void change(std::filesystem::path path,std::vector<std::string> selectedMonitors,int fillmode);
    void changeAll(std::filesystem::path path,int fillmode);
};

#endif // KDECHANGER_H
