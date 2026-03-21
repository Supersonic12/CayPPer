#include<string>
#include "controller.h"
#include <QCoreApplication>
int main(int argc, char* argv[]) {
    if (argc < 1) {
        return 1;
    }
    QCoreApplication::setOrganizationName("caypper");
    QCoreApplication::setApplicationName("caypper");
    Controller controller;
    if (argv[1] == std::string("--restore")) {
        try {
            controller.restoreLastWallpapers();
        } catch (std::runtime_error &e) {
            std::cerr << e.what();
        }
    }
    return 0;
}
