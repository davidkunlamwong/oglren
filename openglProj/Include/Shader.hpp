#pragma once

#include <OpenGL/OpenGL.h>
#include <filesystem>
#include <initializer_list>

namespace glw {

class Shader {
public:
    enum State {
        INVALID,
        READY,
        COMPILED,
    };
    
private:
    GLuint handle;
    
    mutable State state = Shader::State::INVALID;
    
    std::string sourceBuffer;
    const char* sourceBufferPtr;
    
public:
    Shader() = delete;
    Shader(GLenum type, std::filesystem::path pathToSource);
    
    bool compile() const;
    void release() const;
    
    operator GLuint() const;
    
};

class Program {
private:
    GLuint handle;
    
public:
    Program() = delete;
    Program(std::initializer_list<Shader> list);
    
    operator GLuint() const;
};

}
