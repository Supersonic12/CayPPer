#include "changer.h"
#include <cstdlib>
Changer::Changer() {}

int Changer::runXWallpaper(std::filesystem::path chosenPaper){
    if(std::system(("xwallpaper set"+chosenPaper.string()).c_str())==0){
        return 0;
    }
    return 1;
}
