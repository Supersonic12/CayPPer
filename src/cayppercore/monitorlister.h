#ifndef MONITORLISTER_H
#define MONITORLISTER_H

#include<vector>
#include<string>

#include"composfinder.h"
class MonitorLister
{
public:
    MonitorLister();
    std::vector<std::string> getMonitorWayland(ComposFinder::Compositor compositor) const;
    std::vector<std::string> getMonitorX() const;

};

#endif // MONITORLISTER_H
