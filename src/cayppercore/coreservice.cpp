#include "coreservice.h"
#include "domainExpansion/fillmodeconverter.h"
#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>
coreService::coreService() :
    isWayland_(envvardetector_.isWayland()),
    isXFCE_(envvardetector_.isXFCE()),
    compositor_(envvardetector_.getCompositor())
{
}

bool coreService::isWayland() const{
    return isWayland_;
}
EnvVarDetector::Compositor coreService::compositor() const{
    return compositor_;
}

std::vector<std::string> coreService::monitors() const{
    if(!isWayland_){
        std::vector<std::string>monitorlist=monitorlister_.getMonitorX();
        // for(auto &monitor: monitorlist){
        //     std::cout<<monitor<<",\n";
        // }
        return monitorlist;
    }
    return monitorlister_.getMonitorWayland(compositor_);
}

void coreService::setWallpaper(const std::filesystem::path& wallPath,std::vector<std::string>& selectedMonitors,FillMode fillMode){
    if(!isWayland_){
        if(!isXFCE()){
            auto mode=mapToXWall(fillMode);
            if(!mode){
                return;
            }
            changer_.runXWallpaper(wallPath,selectedMonitors,*mode);}
        else{
            auto mode=maptoXFCE(fillMode);
            if(!mode){
                return;
            }
            changer_.runXFCE(wallPath,selectedMonitors,*mode);
        }
    }else{
        if(compositor_==EnvVarDetector::Compositor::Hyprland){
            auto mode=mapToHyprland(fillMode);
            if(!mode){
                return;
            }
            changer_.runHyprland(wallPath,selectedMonitors,*mode);
        }else if(compositor_==EnvVarDetector::Compositor::Sway){
            auto mode=mapToSway(fillMode);
            if(!mode){
                return;
            }
            changer_.runSway(wallPath,selectedMonitors,*mode);
        }
    }
}

std::vector<FillMode> coreService::supportedModes() const{
    if(!isWayland_){
        if(isXFCE_){
            return{
                FillMode::Center,
                FillMode::Tile,
                FillMode::Stretch,
                FillMode::Zoom,
                FillMode::Scaled,
                FillMode::Scaled_Keep_Aspect
            };
        }
        else{
            return{
                FillMode::Center,
                FillMode::Focus,
                FillMode::Maximize,
                FillMode::Stretch,
                FillMode::Tile,
                FillMode::Zoom
            };
        }
    }else if(compositor_==EnvVarDetector::Compositor::Hyprland) {
        return{
            FillMode::Contain,
            FillMode::Cover,
            FillMode::Fill,
            FillMode::Tile
        };
    }else if(compositor_==EnvVarDetector::Compositor::Sway){
        return{
            FillMode::Center,
            FillMode::Fill,
            FillMode::Fit,
            FillMode::Stretch,
            FillMode::Tile
        };
    }else{
        return{};
    }
}

const std::vector<std::filesystem::path> coreService::listDirectory(std::filesystem::path stdpath){
    startWatching(stdpath);
    return directorylister_.listDirectory(stdpath);
}

void coreService::setDirectoryChangeCallBack(std::function<void()> callback){
    directoryChangedCallback_ = std::move(callback);
}
void coreService::notifyDirectoryChanged(){
    if(directoryChangedCallback_){
        directoryChangedCallback_();
    }
}

void coreService::startWatching(const std::filesystem::path &path){
    stopWatching();

    watchedPath_ = path;

    inotifyFD_=inotify_init1(IN_NONBLOCK);
    if (inotifyFD_ <0){
        std::cerr<< "Inotify_init1 failed"<<strerror(errno)<<"\n";
        return;
    }
    watchDescriptor_ =inotify_add_watch(inotifyFD_,watchedPath_.c_str(),IN_CREATE|IN_DELETE|IN_MODIFY|IN_MOVED_FROM|IN_MOVED_TO);
    if(watchDescriptor_<0){
        std::cerr<<"inotify_add_watch failed"<<strerror(errno)<<"\n";
        close(inotifyFD_);
        inotifyFD_=-1;
        return;
    }
    running_=true;
    watcherThread_=std::thread(&coreService::watchLoop,this);

}

void coreService::stopWatching(){
    running_=false;

    if(watcherThread_.joinable()){
        watcherThread_.join();
    }
    if(watchDescriptor_>=0){
        inotify_rm_watch(inotifyFD_,watchDescriptor_);
    }
    if(inotifyFD_>=0){
        close(inotifyFD_);
    }
    watchDescriptor_=-1;
    inotifyFD_=-1;
}
void coreService::watchLoop(){
    const size_t bufLen=1024*(sizeof(struct inotify_event)+16);
    char buffer[bufLen];

    while(running_){
        ssize_t length = read(inotifyFD_,buffer,bufLen);
        if(length<0){
            if(errno==EAGAIN){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }else{
                std::cerr<<"inotify read error"<<strerror(errno)<<"\n";
                break;
            }
        }
        size_t i=0;
        while (i<static_cast<size_t>(length)) {
            struct inotify_event* event = (struct inotify_event* )&buffer[i];
            if(event->len){
                if(directoryChangedCallback_){
                    directoryChangedCallback_();
                }
            }
            i+=sizeof(struct inotify_event)+event->len;
        }
    }
}
coreService::~coreService(){
    stopWatching();
}
bool coreService::isXFCE() const{
    return isXFCE_;
}
