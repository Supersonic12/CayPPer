#ifndef PERSISTENCEADDON_H
#define PERSISTENCEADDON_H

#include <map>
#include <string>
#include "envvardetector.h"
class persistenceAddon
{
public:
    persistenceAddon();
    std::string autostartPath;
    std::map<EnvVarDetector::Compositor,std::string>
        persistenceMap{
            {EnvVarDetector::Compositor::GNOME,""}
        };
};

#endif // PERSISTENCEADDON_H
