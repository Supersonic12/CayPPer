#ifndef MONITORLISTER_H
#define MONITORLISTER_H

#include<vector>
#include<string>

#include"envvardetector.h"
class MonitorLister
{
public:
    MonitorLister();
    std::vector<std::string> getMonitor(EnvVarDetector::Compositor compositor) const;

};

#endif // MONITORLISTER_H
