#include "changerFactory.h"
#include <dlfcn.h>
#include <filesystem>
#include <stdexcept>
#include <functional>
#include <map>
#include "IChanger.h"
static constexpr const char* BACKEND_DIR =
    "/home/jesus/Projects/CayPPer/build/lib/caypper/";/*/usr/lib/caypper/*/
using BackendPtr = std::unique_ptr<IBackend, std::function<void(IBackend*)>>;
BackendPtr ChangerFactory::loadBackend(const std::string& soName) {
    std::string path = std::string(BACKEND_DIR) + soName;
    if (!std::filesystem::exists(path)) {
        return nullptr;
    }
    void* handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!handle) {
        return nullptr;
    }
    auto create = (CreateBackendFn) dlsym(handle, "createBackend");
    auto destroy = (DestroyBackendFn) dlsym(handle, "destroyBackend");
    if (!create || !destroy) {
        dlclose(handle);
        throw std::runtime_error(std::string("ERROR: backend symbols are missing in )" + soName));
    }
    return BackendPtr(
               create(),
    [handle, destroy](IBackend * b) {
        if (b) {
            destroy(b);
        }
        dlclose(handle);
    });
}
BackendPtr ChangerFactory::create(EnvVarDetector::Compositor compositor, bool isWayland) {
    std::string libName;
    if (!isWayland) {
        libName = compositor == EnvVarDetector::Compositor::XFCE ? "xfcechanger.so" : "xelevenchanger.so";
    } else {
        static const std::map<EnvVarDetector::Compositor, std::string>
        waylandLibs = {
            {EnvVarDetector::Compositor::GNOME, "gnomechanger.so"},
            {EnvVarDetector::Compositor::KDE, "plasmachanger.so"},
            {EnvVarDetector::Compositor::Hyprland, "hyprlandchanger.so"},
            {EnvVarDetector::Compositor::Sway, "swaychanger.so"}
        };
        auto it = waylandLibs.find(compositor);
        if (it != waylandLibs.end()) {
            libName = it->second;
        }
    }
    if (libName.empty()) {
        throw std::runtime_error(std::string("ERROR: Unknown Compositor."));
    }
    auto b = loadBackend(libName);
    if (!b) {
        throw std::runtime_error(std::string("ERROR: " + libName + " backend plugin isn't installed"));
    }
    return b;
}
