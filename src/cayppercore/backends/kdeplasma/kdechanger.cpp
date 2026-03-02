#include "kdechanger.h"
#include <nlohmann/json.hpp>
#include <sdbus-c++/sdbus-c++.h>
#include <../../domainExpansion/jsscriptloader.h>
using json = nlohmann::json;
std::string getJSPATH(){
    if(std::filesystem::exists(JS_TEMPLATE_PATH)){
        return JS_TEMPLATE_PATH;
    }
    return "./script.js";
}
void KDEChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors,FillMode fillMode){
    if(selectedMonitors.empty()){
        std::cerr<<"No Monitor Checked, Check at least one!\n";
        return;
    }
    int int_mode;
    if(auto mapped=mapToKDE(fillMode)){
        int_mode=fromKDEModetoInt(*mapped);
    }else{
        std::cerr<<"Unsupported KDE fillMode!\n";
        return;
    }

    std::vector<int> intVectorMon;
    for(auto const& monitor:selectedMonitors){
        intVectorMon.push_back(std::stoi(monitor));
    }
    json givenVars;
    givenVars["monitors"]=intVectorMon;
    givenVars["wallpaperPath"]="file://"+path.string();
    givenVars["fillMode"]=int_mode;
    auto connection =sdbus::createSessionBusConnection();
    sdbus::ServiceName destination("org.kde.plasmashell");
    sdbus::ObjectPath objPath("/PlasmaShell");
    auto proxy=sdbus::createProxy(*connection,destination,objPath);
    sdbus::InterfaceName interf("org.kde.PlasmaShell");
    sdbus::MethodName scriptEv("evaluateScript");

    std::string jsScript=loadScript("script.js");
    /* CAUTION for now i hardcode path of script js and when js file copied
     *          to that path manually it works like a charm
     *          i should make it that cmake automatically puts js
     *          file to the place i want
     *          /usr/share/applications/caypper/script.js might be the path i would use
     *          /usr/share/caypper/script.js  also is possible  */
    jsScript+="\napply("+givenVars.dump()+");";
    proxy->callMethod(scriptEv).onInterface(interf).withArguments(jsScript);
}
void KDEChanger::setWallpaperAll(std::filesystem::path path,FillMode fillMode){
    int int_mode;
    if(auto mapped=mapToKDE(fillMode)){
        int_mode=fromKDEModetoInt(*mapped);
    }else{
        std::cerr<<"Unsupported KDE fillMode!\n";
        return;
    }
    json givenVars;
    givenVars["wallpaperPath"]="file://"+path.string();
    givenVars["fillMode"]=int_mode;
    auto connection = sdbus::createSessionBusConnection();
    sdbus::ServiceName destination("org.kde.plasmashell");
    sdbus::ObjectPath objpath("/PlasmaShell");
    auto proxy = sdbus::createProxy(*connection,destination,objpath);
    sdbus::InterfaceName interf("org.kde.PlasmaShell");
    sdbus::MethodName scriptEvAll("evaluateScript");
    std::string jsScript=loadScript(getJSPATH());
    jsScript+="\napplyAll("+givenVars.dump()+")";
    proxy->callMethod(scriptEvAll).onInterface(interf).withArguments(jsScript);
}
