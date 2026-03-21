#ifndef GETCONFIGPATH_H
#define GETCONFIGPATH_H
#include <filesystem>
inline std::filesystem::path getConfigPath() {
    const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
    const char* home = std::getenv("HOME");
    std::filesystem::path configPath;
    if (xdgConfig) {
        configPath = std::filesystem::path(xdgConfig) / "caypper";
    } else {
        configPath = std::filesystem::path(home) / ".config" / "caypper";
    }
    if (!std::filesystem::exists(configPath)) {
        std::filesystem::create_directory(configPath);
    }
    return configPath;
}
#endif // GETCONFIGPATH_H
