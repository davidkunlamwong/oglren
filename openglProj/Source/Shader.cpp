#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

#include "Assert.hpp"

using namespace glw;

Shader::Shader(GLenum type, std::filesystem::path pathToSource) {
    auto file = std::fstream(pathToSource);
    if (not file)
    {
        throw std::runtime_error("Shader source not found!");
    }
    std::stringstream sourceStream;
    sourceStream << file.rdbuf();
    sourceBuffer = sourceStream.str();
    sourceBufferPtr = sourceBuffer.c_str();
    
    handle = glCreateShader(type);
    
    glShaderSource(handle, 1, &sourceBufferPtr, NULL);
    
    state = Shader::State::READY;
}

Shader::operator GLuint() const {
    Assert(state == Shader::State::COMPILED);
    
    return handle;
}

bool Shader::compile() const {
    Assert(state == Shader::State::READY);
    
    glCompileShader(handle);
    
    GLint result;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
    
    if (result == GL_TRUE) {
        state = Shader::State::COMPILED;
    }
    else {
        auto logBuffer = std::string{"", 512};
        glGetShaderInfoLog(handle, 512, NULL, logBuffer.data());
        std::cout << "Shader Compilation Error: " << logBuffer << std::endl;
    }
    
    return static_cast<bool>(result);
}

void Shader::release() const{ 
    glDeleteShader(handle);
    state = Shader::State::INVALID;
}

Program::Program(std::initializer_list<Shader> list) {
    handle = glCreateProgram();
    
    for (auto& shader: list) {
        glAttachShader(handle, shader);
    }
    
    glLinkProgram(handle);
    
    GLint result;
    glGetProgramiv(handle, GL_LINK_STATUS, &result);
    
    if (result == GL_FALSE) {
        auto logBuffer = std::string{"", 512};
        glGetShaderInfoLog(handle, 512, NULL, logBuffer.data());
        std::cout << "Shader Link Error: " << logBuffer << std::endl;
        throw std::runtime_error("Shader link error!");
    }
    
    for (auto& shader: list) {
        shader.release();
    }
}

Program::operator GLuint() const{
    return handle;
}
