#include "kdemonitorlisting.h"
#include <sdbus-c++/sdbus-c++.h>
#include <regex>
#include <iostream>
kdemonitorlisting::kdemonitorlisting() {}
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
int kdemonitorlisting::getMonitorCount(){
    std::regex numberOfScreens(R"(Number of Screens: (\d+):\n-+\n)");
    return 0;
}

std::vector<int> kdemonitorlisting::getMonitorIDs(){
    std::regex screenIDs(R"(Screen (\d+))");
    std::smatch matches;
    if(std::regex_search(kwinInfo_,matches,screenIDs)){
        std::cout<<"Pattern Found: "<<std::endl;
        std::cout<<"Matched Text: "<< matches[0] <<std::endl;
    }else{
        std::cout<<"Pattern Not Found.\n";
    }
    return {};
}
std::vector<std::string> kdemonitorlisting::getMonitorNames(){
    std::regex screenNames(R"(Screen\s\d+:\n-+\nName:\s([^\n]+))");
    return {};
}
bool kdemonitorlisting::isEnabled(){
    std::regex screenEnabled(R"(Screen\s\d+:\n-+\nName:\s[^\n]+\nEnabled:\s(\d+))");
    return 0;
}
