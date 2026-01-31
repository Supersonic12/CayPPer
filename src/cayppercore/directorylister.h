#ifndef DIRECTORYLISTER_H
#define DIRECTORYLISTER_H

#include <filesystem>
#include <vector>

const std::vector<std::filesystem::path> listDirectory(std::filesystem::path dirPath);

#endif
