#include "kdechanger.h"
#include <nlohmann/json.hpp>
#include <sdbus-c++/sdbus-c++.h>
#include <../../domainExpansion/jsscriptloader.h>
#include "kdemonitorlisting.h"
using json = nlohmann::json;
std::string KDEChanger::getJSPATH(){
    if(std::filesystem::exists(JS_TEMPLATE_PATH)){
        return JS_TEMPLATE_PATH;
    }
    return "./script.js";
}
void KDEChanger::setWallpaper(std::filesystem::path path,std::vector<std::string> selectedMonitors,FillMode fillMode){
    if(selectedMonitors.empty()){
        throw std::runtime_error(std::string("Warning: No Monitor Checked, Check at least one!"));
    }
    int int_mode;
    if(auto mapped=mapToKDE(fillMode)){
        int_mode=fromKDEModetoInt(*mapped);
    }else{
        throw std::runtime_error(std::string("ERROR: mapToKDE failed, Unsupported fill mode"));
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

    std::string jsScript=loadScript(getJSPATH());
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
        throw std::runtime_error(std::string("ERROR: mapToKDE failed, Unsupported fill mode"));
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
std::vector<FillMode> KDEChanger::supportedModes() const{
    return{
        FillMode::Center,
        FillMode::Scaled,
        FillMode::ScaledCropped,
        FillMode::ScaledKeepAspect,
        FillMode::Tile,
        FillMode::TileHorizontally,
        FillMode::TileVertically
    };
}
std::vector<std::string> KDEChanger::monitors() const{
    kdemonitorlisting listing;
    std::vector<int> ids= listing.getMonitorListIDs();
    std::vector<std::string> str_ids;
    for(int id: ids){
        str_ids.push_back(std::to_string(id));
    }
    return str_ids;
}
