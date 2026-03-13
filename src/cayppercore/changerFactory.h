#pragma once
#ifndef CHANGER_FACTORY_H
#define CHANGER_FACTORY_H
#include <memory>
#include "IChanger.h"
#include "envvardetector.h"
#include <functional>
class ChangerFactory{
public:
    using BackendPtr = std::unique_ptr<IBackend,std::function<void(IBackend*)>>;
    BackendPtr create(EnvVarDetector::Compositor compositor,bool isWayland);
private:
    BackendPtr loadBackend(const std::string& soName);
};

#endif
