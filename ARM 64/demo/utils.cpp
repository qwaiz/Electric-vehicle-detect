#include <string>
#include <dirent.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <string.h>

int read_files_in_dir(std::string dir, std::vector<std::string> &file_names)
{

    //DIR *p_dir = opendir("./img");
    DIR *p_dir = opendir(dir.c_str());
    if (p_dir == nullptr)
    {
        printf("read_files_in_dir nullptr\n");
        return -1;
    }

    struct dirent *p_file = nullptr;
    while ((p_file = readdir(p_dir)) != nullptr)
    {
        if ( strcmp(p_file->d_name,".") != 0 &&      strcmp(p_file->d_name, "..") != 0)
        {
            std::string cur_file_name(p_file->d_name);
            file_names.push_back(cur_file_name);
        }
    }

    closedir(p_dir);
    return 0;
}
