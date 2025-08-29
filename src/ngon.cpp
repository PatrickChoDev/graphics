//
// Created by Thanapat Chotipun on 15/8/2025 AD.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <unistd.h>
#include "handler/initializer.hpp"
#include "handler/shader.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

int main() {
    GLFWwindow* window = initialize_window();

    // Simple shader setup assumed here
    GLuint shader_program = create_shader_program(
      "shader/vertex_shader.glsl",
      "shader/fragment_shader.glsl"
    );

    std::vector<float> vertices = {
        // Center vertex (not used for line loop)
        0.0f,  0.0f, 0.0f,

        // Hexagon perimeter vertices (6 vertices)
        0.5f,  0.0f, 0.0f,
        0.25f,  0.433f, 0.0f,
       -0.25f,  0.433f, 0.0f,
       -0.5f,  0.0f, 0.0f,
       -0.25f, -0.433f, 0.0f,
        0.25f, -0.433f, 0.0f,

    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);

        // Draw only the 6 perimeter vertices starting at index 1
        glDrawArrays(GL_TRIANGLE_FAN, 1, 6);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    cleanup_window(window);

    return 0;
}