#ifndef KDECHANGER_H
#define KDECHANGER_H
#include <vector>
#include <string>
#include <filesystem>
class kdechanger
{
public:
    kdechanger();
    void change(std::filesystem::path path,std::vector<std::string> selectedMonitors,std::string fillmode);
};

#endif // KDECHANGER_H
