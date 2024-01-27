#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

#include "Shader.hpp"

float vertices[] = {
     1.0f,  1.0f, 0.0f,  // top right
     1.0f, -1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, const char * argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(800, 800, "OGL", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    auto vertexShader = glw::Shader(GL_VERTEX_SHADER, "./Shader/vertexShader.vert");
    vertexShader.compile();
    
    auto fragmentShader = glw::Shader(GL_FRAGMENT_SHADER, "./Shader/raymarch.frag");
    fragmentShader.compile();
    
    auto shaderProgram = glw::Program{vertexShader, fragmentShader};
    
    auto timeUniformLocation = glGetUniformLocation(shaderProgram, "time");
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    
    auto start = std::chrono::system_clock::now();
    auto current = std::chrono::system_clock::now();
    auto last = current;
    std::chrono::duration<double> elapsed = current - start;
    std::chrono::duration<double> frameTime = current - last;
    
    while (!glfwWindowShouldClose(window)) {
        current = std::chrono::system_clock::now();
        elapsed = current - start;
        
//        frameTime = current - last;
//        last = current;
//        std::cout << 1.0f / frameTime.count() << " fps\n";
        
        glClearColor(0.13f, 0.04f, 0.022f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // ..:: Drawing code (in render loop) :: ..
        // 4. draw the object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        
        glUniform1f(timeUniformLocation, elapsed.count());
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
