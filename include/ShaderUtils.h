#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <GL/glew.h>
#include <string>

GLuint compileShader(GLenum type, const char* source);
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
std::string loadShaderSource(const std::string& filepath);

#endif
