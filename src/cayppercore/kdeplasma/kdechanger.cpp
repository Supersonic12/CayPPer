#include "kdechanger.h"
#include <nlohmann/json.hpp>
#include <sdbus-c++/sdbus-c++.h>
#include <../domainExpansion/jsscriptloader.h>
using json = nlohmann::json;
kdechanger::kdechanger() {}
void kdechanger::change(std::filesystem::path path,std::vector<std::string> selectedMonitors,int fillmode){
    std::vector<int> intVectorMon;
    for(auto const& monitor:selectedMonitors){
        intVectorMon.push_back(std::stoi(monitor));
    }
    json givenVars;
    givenVars["monitors"]=intVectorMon;
    givenVars["wallpaperPath"]="file://"+path.string();
    givenVars["fillMode"]=fillmode;
    auto connection =sdbus::createSessionBusConnection();
    sdbus::ServiceName destination("org.kde.plasmashell");
    sdbus::ObjectPath objPath("/PlasmaShell");
    auto proxy=sdbus::createProxy(*connection,destination,objPath);
    sdbus::InterfaceName interf("org.kde.PlasmaShell");
    sdbus::MethodName scriptEv("evaluateScript");
    std::string jsScript=loadScript("script.js");
    jsScript+="\napply("+givenVars.dump()+");";
    proxy->callMethod(scriptEv).onInterface(interf).withArguments(jsScript);
}
void kdechanger::changeAll(std::filesystem::path path,int fillmode){
    json givenVars;
    givenVars["wallpaperPath"]="file://"+path.string();
    givenVars["fillMode"]=fillmode;
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ServiceName destination("org.kde.plasmashell");
    sdbus::ObjectPath objpath("/PlasmaShell");
    auto proxy = sdbus::createProxy(*connection,destination,objpath);
    sdbus::InterfaceName interf("org.kde.PlasmaShell");
    sdbus::MethodName scriptEvAll("evaluateScript");
    std::string jsScript=loadScript("script.js");
    jsScript+="\napplyAll("+givenVars.dump()+")";
    proxy->callMethod(scriptEvAll).onInterface(interf).withArguments(jsScript);
}
