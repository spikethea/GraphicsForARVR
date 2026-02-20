#pragma once
#include <vector>
#include <stack>

#define _USE_MATH_DEFINES
#include <cmath>
#include <corecrt_math_defines.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>


using namespace std;

struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, radians; };

class Mesh {
public:
    // pure float number values for OpenGL
    vector<float> lines;
    vector<unsigned int> indices;

    // Constructor
    void init();

    // Destructor
    void release();

    float DegToRad(float degrees);

    void uploadMeshToGPU();

    void updateVertices(const std::vector<Vec3>& vertices);

    void DrawMesh() const;

private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    GLsizei indexCount = 0;
};
