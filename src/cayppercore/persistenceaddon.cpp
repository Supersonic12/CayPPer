#include "persistenceaddon.h"
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include <filesystem>
#include "domainExpansion/fillmodeconverter.h"
#include "domainExpansion/monState.h"
#include <fstream>
#include <functional>
#include "domainExpansion/getConfigPath.h"
using json = nlohmann::json;
persistenceAddon::persistenceAddon() {}

void persistenceAddon::readPersistenceState() {
    std::filesystem::path configPath = std::filesystem::path(getConfigPath()) / "caypper.ini";
    if (!std::filesystem::exists(configPath)) {
        throw std::runtime_error(std::string("ERROR: config file doesn't exist"));
    }
    std::ifstream ifs(configPath);
    if (!ifs.is_open()) {
        throw std::runtime_error(std::string("ERROR: couldn't open config file"));
    }
    std::string targetSection = "[Behavior]";
    std::string targetKey = "persistence";
    std::string line;
    bool inSection = false;
    std::string value;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        line.erase(0, line.find_first_not_of(" \t"));
        if (line[0] == '[' && line.find(']') != std::string::npos) {
            if (line.find(targetSection) != std::string::npos) {
                inSection = true;
            } else {
                inSection = false;
            }
            continue;
        }
        if (inSection) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                if (key == targetKey) {
                    value = line.substr(pos + 1);
                    break;
                }
            }
        }
    }
    ifs.close();
    if (value == "true") {
        persistenceState_ = true;
    } else {
        persistenceState_ = false;
    }
    if (value.empty()) {
        throw std::runtime_error(std::string("ERROR: key or value couldn't found"));
    }
}
bool persistenceAddon::getPersistenceState() {
    readPersistenceState();
    return persistenceState_;
}
void persistenceAddon::readLastSetupJson() {
    std::filesystem::path lastSetupPath = std::filesystem::path(getConfigPath()) / "lastSetup.json";
    if (!std::filesystem::exists(lastSetupPath)) {
        throw std::runtime_error(std::string("ERROR: lastSetup record doesn't exist."));
    }
    std::ifstream ifs(lastSetupPath);
    if (!ifs.is_open()) {
        throw std::runtime_error(std::string("ERROR: couldn't open lastSetup record"));
    }
    json dataList;
    lastStatesVector_.clear();

    try {
        dataList = json::parse(ifs);
    } catch (json::parse_error &e) {
        std::cerr << "ERROR: failed to parse from lastSetup.json";
        dataList = json::object();
    }
    for (auto& [monitorName, MonitorData] : dataList["processInfo"].items()) {
        stateOfMon state;
        std::string wallPath = MonitorData["wallPath"];
        std::string fillMode = MonitorData["fillMode"];
        state.name = monitorName;
        state.fillMode = toFillMode(fillMode);
        state.wallPath = wallPath;
        lastStatesVector_.push_back(state);
    }
}
void persistenceAddon::setLastSetupJson(stateOfMons& states) {
    std::filesystem::path lastSetupPath = std::filesystem::path(getConfigPath()) / "lastSetup.json";
    std::filesystem::path tempPath = std::filesystem::path(getConfigPath()) / "lastSetup.json.tmp";
    json newJson;
    if (std::filesystem::exists(lastSetupPath)) {
        std::ifstream ifs(lastSetupPath);
        if (!ifs.is_open()) {
            throw std::runtime_error("ERROR: lastSetup.json exists, but couldn't open for reading");
        }
        try {
            newJson = json::parse(ifs);
        } catch (json::parse_error &e) {


            std::cerr <<
            "ERROR: lastSetup.json exists, but couldn't parse to json, will overwrite to it\n";
            newJson = json::object();
        }
    } else {
        std::cerr << "INFO: lastSetup.json doesn't exist, app will create new one\n";
    }
    std::ofstream ofs(tempPath);
    if (!ofs.is_open()) {
        throw std::runtime_error(
            std::string("ERROR: couldn't create or open temporary file for changing last setup"));
    }
    for (auto const& mon : states.name) {
        newJson["processInfo"][mon] = {
            {"fillMode", fromModetoString(states.fillMode)},
            {"wallPath", states.wallPath.string()}
        };
    }
    ofs << newJson.dump(4) << std::endl;
    ofs.close();
    std::filesystem::rename(tempPath, lastSetupPath);
}
std::vector<stateOfMon> persistenceAddon::getLastSetupJson() {
    return lastStatesVector_;
}
void persistenceAddon::restoreLastWallpapers() {
    //setLastStatesVector();
    readLastSetupJson();
    if (onRestoreFound) {
        for (stateOfMon &state : lastStatesVector_) {
            stateOfMons monsState = monToMons(state);
            onRestoreFound(monsState);
        }
    }

}
void persistenceAddon::setPersistenceState(bool enabled) {
    std::filesystem::path configPath = std::filesystem::path(getConfigPath()) / "caypper.ini";
    std::filesystem::path tempPath = std::filesystem::path(getConfigPath()) / "caypper.ini.tmp";
    std::ifstream ifs(configPath);
    std::ofstream ofs(tempPath);
    if (!ifs.is_open() || !ofs.is_open()) {
        throw std::runtime_error(std::string("ERROR: couldn't open files for processing"));
    }
    std::string newState = enabled ? "true" : "false";
    std::string line;
    std::string targetSection = "[Behavior]";
    std::string targetKey = "persistence";
    bool inTargetSection = false;
    bool keyWritten = false;
    while (std::getline(ifs, line)) {
        std::string trimmed = line;
        //line is comment, continue with next line
        if (trimmed.empty() || trimmed[0] == '#' || trimmed[0] == ';') {
            ofs << trimmed << std::endl;
            continue;
        }
        //erase unwanted spaces
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));

        // section header
        if (trimmed[0] == '[' && trimmed.find(']') != std::string::npos) {
            // leaving target section without having written the key yet
            if (inTargetSection && !keyWritten) {
                ofs << targetKey << '=' << newState << '\n';
                keyWritten = true;
            }
            inTargetSection = (trimmed.find(targetSection) != std::string::npos);
            ofs << trimmed << '\n';
            continue;
        }
        if (inTargetSection) {
            size_t pos = trimmed.find('=');
            if (pos != std::string::npos && trimmed.substr(0, pos) == targetKey) {
                ofs << targetKey << '=' << newState << '\n';
                keyWritten = true;
                continue; // skip original line
            }
        }
        ofs << line << '\n';
    }
    if (inTargetSection && !keyWritten) {
        ofs << targetKey << '=' << newState << '\n';
        keyWritten = true;
    }

    // [Behavior] section never existed at all
    if (!keyWritten) {
        ofs << '\n' << targetSection << '\n'
            << targetKey << '=' << newState << '\n';
    }
    ifs.close();
    ofs.close();
    std::filesystem::rename(tempPath, configPath);
}
