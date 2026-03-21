#ifndef PERSISTENCEADDON_H
#define PERSISTENCEADDON_H
#include "domainExpansion/monState.h"
#include <functional>
class persistenceAddon {
  public:
    persistenceAddon();
    void setPersistenceState(bool enabled);
    bool getPersistenceState();
    void readPersistenceState();
    void readLastSetupJson();
    void setLastSetupJson(stateOfMons& state);
    std::vector<stateOfMon> getLastSetupJson();
    void restoreLastWallpapers();
    std::function<void(const stateOfMons&)> onRestoreFound;
  private:
    bool persistenceState_;
    std::vector<stateOfMon> lastStatesVector_;
};

#endif // PERSISTENCEADDON_H
