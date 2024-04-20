#ifndef UTIL_H
#define UTIL_H

#include <GLFW/glfw3.h>
#include <fstream>

inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

inline std::string read_file(std::string path_to_file)
{
    std::ifstream file(path_to_file);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{}};
}

#endif
