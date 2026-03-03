#ifndef RAIIGUARD_H
#define RAIIGUARD_H
#include <spawn.h>
#include <sys/wait.h>
class RAIIGuard
{
public:
    explicit RAIIGuard(int FD){
        this->fd=FD;
    }
    ~RAIIGuard(){ if (fd>=0) close(fd);};
    void release(){
        if(fd>=0){
            close(fd);
            fd=-1;
        }
    }
    RAIIGuard(const RAIIGuard&)=delete;
    RAIIGuard operator=(const RAIIGuard&)=delete;
    RAIIGuard(RAIIGuard&& other) noexcept:fd(other.fd){
        other.fd=-1;
    }
    RAIIGuard &operator=(RAIIGuard && other) noexcept;
private:
    int fd;
};

#endif // RAIIGUARD_H
