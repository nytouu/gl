#ifndef UTIL_H
#define UTIL_H

#include <GLFW/glfw3.h>
#include <fstream>

const float SCREEN_RATIO = 16.0f / 9.0f;

inline std::string read_file(std::string path_to_file)
{
    std::ifstream file(path_to_file);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}};
}

#endif
