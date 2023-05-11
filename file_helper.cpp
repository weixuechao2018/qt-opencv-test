#include "file_helper.h"

#include <stdio.h>
#include <cstdio>
#include <dirent.h>
#include <type_traits>

#include <string>
#include <vector>

file_helper::file_helper()
{

}

bool file_helper::readFile(std::string &filepath, unsigned char *buffer) {
    if(buffer) {
        return false;
    }
    IfstreamSimpleUnit ifstreamSimpleUnit(filepath);
    return ifstreamSimpleUnit.read(buffer);
}

bool file_helper::getDirFile(std::string &folder_path, std::vector<std::string> &file_vector) {
    struct dirent *dirp;
    DIR *dir = opendir((char *)folder_path.data());
    if(dir == nullptr) {
        return false;
    }
    while ((dirp = readdir(dir)) != nullptr) {
        std::string file_name(dirp->d_name);
        if (file_name == "." || file_name == ".." || file_name == ".DS_Store") {
            continue;
        }
        file_vector.push_back(folder_path + file_name);
    }
    return true;
}

