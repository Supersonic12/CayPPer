#ifndef COMPOSFINDER_H
#define COMPOSFINDER_H
#include<filesystem>
class ComposFinder
{
public:
    ComposFinder();
    void isWayland(std::filesystem::path chosenPaper);
    const char* getCompositor();
};

#endif // COMPOSFINDER_H
