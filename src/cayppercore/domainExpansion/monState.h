#ifndef MONSTATE_H
#define MONSTATE_H
#include <string>
#include <filesystem>
#include <vector>
#include "fillmode.h"
struct stateOfMons{
    std::vector<std::string> name;
    std::filesystem::path wallPath;
    FillMode fillMode;
};

#endif // MONSTATE_H
