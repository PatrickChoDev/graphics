#include <GLFW/glfw3.h>
// #include <glad/glad.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include <sstream>

// Shader loading utility
std::string LoadShaderSource(const char* filename) {
    const std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open shader: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void checkShader(GLuint shader, const std::string& type) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, NULL, info);
        std::cerr << type << " shader compilation error:\n" << info << std::endl;
    }
}

void checkProgram(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        std::cerr << "Program link error:\n" << info << std::endl;
    }
}

// Fullscreen quad
float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLSL Shader", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Create and compile shader
    std::string fragSrc = LoadShaderSource("shader.frag");
    std::string vertSrc = R"(#version 330 core
        layout (location = 0) in vec2 pos;
        layout (location = 1) in vec2 tex;
        out vec2 fragCoord;
        void main() {
            fragCoord = tex * vec2(800, 600);
            gl_Position = vec4(pos, 0.0, 1.0);
        }
    )";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char* vsrc = vertSrc.c_str();
    glShaderSource(vs, 1, &vsrc, nullptr);
    glCompileShader(vs);
    checkShader(vs, "Vertex");

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fsrc = fragSrc.c_str();
    glShaderSource(fs, 1, &fsrc, nullptr);
    glCompileShader(fs);
    checkShader(fs, "Fragment");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    checkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Setup quad
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);       // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // tex
    glEnableVertexAttribArray(1);

    // Time tracking
    auto start = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float>(now - start).count();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(glGetUniformLocation(shaderProgram, "iTime"), time);
        glUniform2f(glGetUniformLocation(shaderProgram, "iResolution"), width, height);
        glUniform4f(glGetUniformLocation(shaderProgram, "iMouse"), mx, my, 0.0, 0.0); // Simple mouse

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
