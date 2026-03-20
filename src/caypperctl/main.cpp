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

        controller.restoreLastWallpapers();
    }
    return 0;
}
