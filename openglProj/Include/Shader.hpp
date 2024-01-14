#pragma once

#include <OpenGL/OpenGL.h>
#include <filesystem>

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
    void release();
    
    operator GLuint() const;
    
};

}

//class Program {
//public:
//    Program
//}
