#ifndef KDEMONITORLISTING_H
#define KDEMONITORLISTING_H
#include <vector>
#include <string>
class kdemonitorlisting
{
public:
    kdemonitorlisting();
    std::vector<int> getMonitorIDs();
    std::vector<std::string> getMonitorNames();
    int getMonitorCount();
    bool isEnabled();
    void setKWinInfo();
private:
    std::string kwinInfo_;

    //in Constructor setKWinInfo will be called
    //when called getMonitorNames will parse kwinInfo take names of enabled monitors and give them to coreservice i guess
    //yeap in coreservice if KDE PLASMA 6.4.5 it will create an object of kdemonitorlisting class

};

#endif // KDEMONITORLISTING_H
