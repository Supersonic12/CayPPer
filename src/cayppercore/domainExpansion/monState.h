#ifndef MONSTATE_H
#define MONSTATE_H
#include <string>
#include <filesystem>
#include <vector>
#include "fillmode.h"
//will be used to pass argument to setWallpaper functions
struct stateOfMons {
    std::vector<std::string> name;
    std::filesystem::path wallPath;
    FillMode fillMode;
};
//will be used to store each monitor's state to lastSetup.json
struct stateOfMon {
    std::string name;
    std::filesystem::path wallPath;
    FillMode fillMode;
};

inline stateOfMons monToMons(stateOfMon& state) {
    stateOfMons newMons;
    newMons.fillMode = state.fillMode;
    newMons.wallPath = state.wallPath;
    newMons.name.push_back(state.name);
    return newMons;
}

// inline stateOfMon monsToMon(stateOfMons& state){
//     stateOfMon newMon;

// }
#endif // MONSTATE_H
