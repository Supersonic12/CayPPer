#pragma once
#ifndef CHANGER_FACTORY_H
#define CHANGER_FACTORY_H
#include <memory>
#include "IChanger.h"
#include "envvardetector.h"
class ChangerFactory{
public:
    static std::unique_ptr<IBackend>
    create(EnvVarDetector::Compositor compositor,bool isWayland);
};

#endif
