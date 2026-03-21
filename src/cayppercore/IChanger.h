#ifndef ICHANGER_H
#define ICHANGER_H
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include "domainExpansion/fillmode.h"
#include "domainExpansion/fillmodeconverter.h"
#include "domainExpansion/monState.h"
class IBackend{
public:
    virtual ~IBackend()=default;
    virtual void setWallpaper(stateOfMons* newState)=0;
    virtual std::vector<FillMode> supportedModes() const=0;
    virtual std::vector<std::string> monitors() const=0;
};

extern "C" {
typedef IBackend* (*CreateBackendFn)();
typedef void      (*DestroyBackendFn)(IBackend*);
}
#endif // ICHANGER_H
