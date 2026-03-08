#ifndef KDEMONITORLISTING_H
#define KDEMONITORLISTING_H

#include <vector>
#include <string>
class kdemonitorlisting
{
public:
    kdemonitorlisting();
    struct Monitor{
        int id;
        std::string name;
        bool operator==(const Monitor&other) const
        {
            return id==other.id && name==other.name;
        }
    };

    std::vector<Monitor> getMonitorList() const;
    int getMonitorCount() const;
    std::vector<int> getMonitorListIDs() const;
    std::vector<std::string> getMonitorListNames() const;
    void setKWinInfo();
    void setMonitorList();
    void setMonitorCount();

private:
    std::string kwinInfo_;
    std::vector<Monitor> monitorList_;
    int monitorCount_=0;
    //in Constructor setKWinInfo will be called
    //when called getMonitorNames will parse kwinInfo take names of enabled monitors and give them to coreservice i guess
    //yeap in coreservice if KDE PLASMA 6.4.5 it will create an object of kdemonitorlisting class

};

#endif // KDEMONITORLISTING_H
