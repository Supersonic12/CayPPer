#ifndef MONITORHANDLER_H
#define MONITORHANDLER_H

#include<vector>
#include<string>

#include"composfinder.h"
class MonitorHandler
{
public:
    MonitorHandler();
    std::vector<std::string> getMonitor(ComposFinder::Compositor compositor) const;

};

#endif // MONITORHANDLER_H
