//
// Created by Thanapat Chotipun on 29/8/2025 AD.
// Fixed version with proper initialization and resource management
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include "camera.h" // from LearnOpenGL
#include "shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 20;
const float CELL_SIZE = 2.0f;

unsigned int cubeVAO = 0, cubeVBO = 0;
void initCube() {
    float vertices[] = {
        // positions          // normals
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void cleanupCube() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
}

// Globals
Camera camera(glm::vec3(1.5f, 0.0f, 1.5f)); // Camera at floor level
bool keys[1024];
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
int pelletsRemaining = 0;

enum CellType { EMPTY, WALL, PELLET };
CellType map[MAP_WIDTH][MAP_HEIGHT];

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
        keys[key] = (action != GLFW_RELEASE);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// Movement + Collision with improved collision detection
bool canMoveTo(glm::vec3 pos) {
    // Add collision radius/buffer around player
    float collisionRadius = 0.01f;

    // Check multiple points around the player
    glm::vec3 checkPoints[] = {
        pos,                                          // Center
        pos + glm::vec3(collisionRadius, 0, 0),      // Right
        pos + glm::vec3(-collisionRadius, 0, 0),     // Left
        pos + glm::vec3(0, 0, collisionRadius),      // Forward
        pos + glm::vec3(0, 0, -collisionRadius),     // Back
        pos + glm::vec3(collisionRadius, 0, collisionRadius),   // Front-right
        pos + glm::vec3(-collisionRadius, 0, collisionRadius),  // Front-left
        pos + glm::vec3(collisionRadius, 0, -collisionRadius),  // Back-right
        pos + glm::vec3(-collisionRadius, 0, -collisionRadius)  // Back-left
    };

    for (const auto& point : checkPoints) {
        int mx = int(round(point.x));
        int mz = int(round(point.z));
        if (mx < 0 || mx >= MAP_WIDTH || mz < 0 || mz >= MAP_HEIGHT) return false;
        if (map[mx][mz] == WALL) return false;
    }

    return true;
}

void Do_Movement() {
    glm::vec3 dir(0.0f);
    if (keys[GLFW_KEY_W]) dir += camera.Front;
    if (keys[GLFW_KEY_S]) dir -= camera.Front;
    if (keys[GLFW_KEY_A]) dir -= camera.Right;
    if (keys[GLFW_KEY_D]) dir += camera.Right;

    if (glm::length(dir) > 0) {
        dir.y = 0; // Lock movement to horizontal plane
        dir = glm::normalize(dir);
        glm::vec3 newPos = camera.Position + dir * deltaTime * 3.5f; // Slightly faster movement

        if (canMoveTo(newPos)) {
            camera.Position = newPos;
            camera.Position.y = 0.0f; // Keep camera at floor level
        }
    }
}

// Improved maze generator with better connectivity
void generateMap() {
    srand(time(NULL));

    // Initialize all as walls
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int z = 0; z < MAP_HEIGHT; ++z) {
            map[x][z] = WALL;
        }
    }

    // Create paths and pellets
    pelletsRemaining = 0;
    for (int x = 1; x < MAP_WIDTH - 1; ++x) {
        for (int z = 1; z < MAP_HEIGHT - 1; ++z) {
            if (rand() % 4 != 0) { // 75% chance of being passable
                if (rand() % 3 == 0) { // 33% of passable cells have pellets
                    map[x][z] = PELLET;
                    pelletsRemaining++;
                } else {
                    map[x][z] = EMPTY;
                }
            }
        }
    }

    // Ensure the starting area is clear
    map[1][1] = EMPTY;
    map[2][1] = EMPTY;
    map[1][2] = EMPTY;
    map[2][2] = EMPTY;

    std::cout << "Generated maze with " << pelletsRemaining << " pellets\n";
}

// Optimized rendering function
void renderCubeAt(const float x, const float y, const float z, const glm::vec3 color, const Shader& shader) {
    const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

    shader.setMat4("model", model);
    shader.setVec3("color", color);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Batch rendering for better performance
void renderMaze(const Shader& shader) {
    glBindVertexArray(cubeVAO);

    // Render floor tiles first (with a slight offset)
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int z = 0; z < MAP_HEIGHT; ++z) {
            // Floor tile for all cells
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, -0.51f, z)); // Slightly lower
            model = glm::scale(model, glm::vec3(1.0f, 0.1f, 1.0f)); // Thin floor
            shader.setMat4("model", model);
            shader.setVec3("color", glm::vec3(0.1f, 0.1f, 0.3f)); // Dark blue floor
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // Render walls and pellets
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int z = 0; z < MAP_HEIGHT; ++z) {
            if (map[x][z] == WALL) {
                // Full-height wall from floor to ceiling
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)); // Slightly smaller to avoid Z-fighting
                shader.setMat4("model", model);
                shader.setVec3("color", glm::vec3(0.3f, 0.5f, 0.9f)); // Brighter blue walls
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (map[x][z] == PELLET) {
                // Pellet floating slightly above floor
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, -.2f, z));
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // Small pellets
                shader.setMat4("model", model);
                shader.setVec3("color", glm::vec3(1.0f, 0.9f, 0.2f)); // Bright yellow pellets
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }

    glBindVertexArray(0);
}

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Pacman - Collect all pellets!", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    // OpenGL configuration
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable polygon offset to reduce Z-fighting
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    // Disable face culling for now to fix the backward viewing issue
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    // Initialize resources
    initCube();
    generateMap();

    // Initialize shaders AFTER OpenGL context is created
    Shader cubeShader("shader/cube.vert", "shader/cube.frag");

    // Check if shader compiled successfully (basic check)
    if (!cubeShader.ID) {
        std::cerr << "Failed to create shader program\n";
        cleanupCube();
        glfwTerminate();
        return -1;
    }

    std::cout << "Game started! Use WASD to move, mouse to look around. Collect all " << pelletsRemaining << " pellets!\n";

    // Main game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        glfwPollEvents();
        Do_Movement();

        // Check pellet collection
        int camX = int(round(camera.Position.x));
        int camZ = int(round(camera.Position.z));
        if (camX >= 0 && camX < MAP_WIDTH && camZ >= 0 && camZ < MAP_HEIGHT) {
            if (map[camX][camZ] == PELLET) {
                map[camX][camZ] = EMPTY;
                pelletsRemaining--;
                std::cout << "Pellet collected! Remaining: " << pelletsRemaining << "\n";

                // Win condition
                if (pelletsRemaining == 0) {
                    std::cout << "Congratulations! You collected all pellets!\n";
                    glfwSetWindowShouldClose(window, true);
                }
            }
        }

        // Rendering
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Darker background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up matrices with better near/far planes
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.01f,  // Closer near plane
            100.0f
        );
        glm::mat4 view = camera.GetViewMatrix();

        // Use shader and set matrices
        cubeShader.use();
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        // Render the maze
        renderMaze(cubeShader);

        glfwSwapBuffers(window);
    }

    // Cleanup
    cleanupCube();
    glfwTerminate();
    return 0;
}