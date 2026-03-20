#include "coreservice.h"
#include <sys/inotify.h>
#include <unistd.h>
#include <cstring>
#include "changerFactory.h"
coreService::coreService() :
    isWayland_(envvardetector_.isWayland()),
    compositor_(envvardetector_.getCompositor()) {
    backend_ = factory_.create(compositor_, isWayland_);
    if (backend_ == nullptr) {
        throw std::runtime_error(std::string("ERROR: backend object couldn't created"));
    }
}

bool coreService::isWayland() const {
    return isWayland_;
}
EnvVarDetector::Compositor coreService::compositor() const {
    return compositor_;
}

std::vector<std::string> coreService::monitors() const {
    if (!backend_) {
        throw std::runtime_error(
            std::string("ERROR: backend object doesn't exist."));
    }
    return backend_->monitors();
}

void coreService::setWallpaper(stateOfMons *newState) {
    if (!backend_) {
        throw std::runtime_error(
            std::string("ERROR: backend object doesnt exist!\n")
        );
    }

    backend_->setWallpaper(newState);
    if (getPersState()) {
        persState_.setLastSetupJson(*newState);
    }
}

std::vector<FillMode> coreService::supportedModes() const {
    if (!backend_) {
        throw std::runtime_error(
            std::string("ERROR: backend object doesnt exist!\n")
        );
    }
    return backend_->supportedModes();
}

const std::vector<std::filesystem::path> coreService::listDirectory(std::filesystem::path stdpath) {
    startWatching(stdpath);
    return directorylister_.listDirectory(stdpath);
}

void coreService::setDirectoryChangeCallBack(std::function<void()> callback) {
    directoryChangedCallback_ = std::move(callback);
}
void coreService::notifyDirectoryChanged() {
    if (directoryChangedCallback_) {
        directoryChangedCallback_();
    }
}

void coreService::startWatching(const std::filesystem::path &path) {
    stopWatching();

    watchedPath_ = path;

    inotifyFD_ = inotify_init1(IN_NONBLOCK);
    if (inotifyFD_ < 0) {
        std::cerr << "Inotify_init1 failed" << strerror(errno) << "\n";
        return;
    }
    watchDescriptor_ = inotify_add_watch(inotifyFD_, watchedPath_.c_str(),
                                         IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
    if (watchDescriptor_ < 0) {
        std::cerr << "inotify_add_watch failed" << strerror(errno) << "\n";
        close(inotifyFD_);
        inotifyFD_ = -1;
        return;
    }
    running_ = true;
    watcherThread_ = std::thread(&coreService::watchLoop, this);

}

void coreService::stopWatching() {
    running_ = false;

    if (watcherThread_.joinable()) {
        watcherThread_.join();
    }
    if (watchDescriptor_ >= 0) {
        inotify_rm_watch(inotifyFD_, watchDescriptor_);
    }
    if (inotifyFD_ >= 0) {
        close(inotifyFD_);
    }
    watchDescriptor_ = -1;
    inotifyFD_ = -1;
}
void coreService::watchLoop() {
    const size_t bufLen = 1024 * (sizeof(struct inotify_event) +16);
    char buffer[bufLen];

    while (running_) {
        ssize_t length = read(inotifyFD_, buffer, bufLen);
        if (length < 0) {
            if (errno == EAGAIN) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            } else {
                std::cerr << "inotify read error" << strerror(errno) << "\n";
                break;
            }
        }
        size_t i = 0;
        while (i < static_cast<size_t>(length)) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                if (directoryChangedCallback_) {
                    directoryChangedCallback_();
                }
            }
            i += sizeof(struct inotify_event) + event->len;
        }
    }
}
coreService::~coreService() {
    stopWatching();
}

void coreService::restoreWallpapers() {
    persState_.onRestoreFound = [this](const stateOfMons state) {
        this->setWallpaper(const_cast<stateOfMons*>(&state));
    };
    if (persState_.getPersistenceState()) {
        persState_.restoreLastWallpapers();
    }
}
void coreService::setPersState(bool enabled) {
    persState_.setPersistenceState(enabled);
}
bool coreService::getPersState() {
    return persState_.getPersistenceState();
}
