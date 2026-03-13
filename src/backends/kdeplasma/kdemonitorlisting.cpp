#include "kdemonitorlisting.h"
#include <sdbus-c++/sdbus-c++.h>
#include <regex>
#include <iostream>
kdemonitorlisting::kdemonitorlisting() {
    setKWinInfo();
    setMonitorCount();
    setMonitorList();
}
void kdemonitorlisting::setKWinInfo(){
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ServiceName destination("org.kde.KWin");
    sdbus::ObjectPath objpath("/KWin");
    auto proxyID=sdbus::createProxy(*connection,
                                      std::move(destination),
                                      std::move(objpath)
                                      );
    sdbus::InterfaceName interf("org.kde.KWin");
    sdbus::MethodName getconf("supportInformation");
    proxyID->callMethod(getconf).onInterface(interf).storeResultsTo(kwinInfo_);
}
void kdemonitorlisting::setMonitorCount(){
    std::regex numberOfScreens(R"(Number\sof\sScreens:\s*(\d+))");
    std::smatch match;
    if(std::regex_search(kwinInfo_,match,numberOfScreens)){
        monitorCount_=std::stoi(match[1].str());
    }else{
        std::cout<<"Pattern Not Found.\n";
    }
}


void kdemonitorlisting::setMonitorList(){
    std::regex reg_screenIDs(R"(Screen\s(\d+):\n-+\nName:\s([^\n]+)\nEnabled:\s1)");
    auto match_begin=std::sregex_iterator(kwinInfo_.begin(),kwinInfo_.end(),reg_screenIDs);
    auto match_end=std::sregex_iterator();
    for(std::sregex_iterator i=match_begin;i!=match_end;++i){
        struct Monitor target;
        target.id=std::stoi((*i)[1].str());
        target.name=(*i)[2].str();
        std::vector<Monitor>::iterator it=std::find(monitorList_.begin(),monitorList_.end(),target);
        if(it==monitorList_.end()){
            monitorList_.push_back(target);
        }
    }
}
int kdemonitorlisting::getMonitorCount() const{
    return monitorCount_;
}
std::vector<kdemonitorlisting::Monitor> kdemonitorlisting::getMonitorList() const{
    return monitorList_;
}
std::vector<std::string> kdemonitorlisting::getMonitorListNames() const{
    std::vector<std::string> monitorNames;
    for (auto const &monitor:monitorList_){
        monitorNames.push_back(monitor.name);
    }
    return monitorNames;
}
std::vector<int>    kdemonitorlisting::getMonitorListIDs() const{
    std::vector<int> monitorIDs;
    for (auto const &monitor:monitorList_){
        monitorIDs.push_back(monitor.id);
    }
    return monitorIDs;
}

