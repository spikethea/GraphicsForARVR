#include <vector>
#include <stack>
#define _USE_MATH_DEFINES
#include <cmath>
#include <corecrt_math_defines.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "Mesh.h"



using namespace std;




    void Mesh::init() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        std::cout << "Mesh created: VAO=" << VAO
            << ", VBO=" << VBO
            << ", EBO=" << EBO << std::endl;
    }

    // Destructor
    void Mesh::release() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        std::cout << "Mesh Destroyed: VAO=" << VAO
            << ", VBO=" << VBO
            << ", EBO=" << EBO << std::endl;
    }

    float Mesh::DegToRad(float degrees) {
        return degrees * M_PI / 180;
    }

    void Mesh::uploadMeshToGPU() {
        if (lines.empty() || indices.empty()) {
            std::cerr << "Warning: Mesh has no vertices or indices to upload!" << std::endl;
            return;
        }

        if (lines.size() % 3 != 0) {
            std::cerr << "Warning: lines vector size is not divisible by 3!" << std::endl;
        }

        //Bind Vertex array object
        glBindVertexArray(VAO);
        // 0. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Linking Vertex Attributes location = 0, vec3
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lines.size(), lines.data(), GL_STATIC_DRAW);

        //Bind index array object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        //then set the vertex attributes pointers/ vertex layout
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //GLint vaoID;
        //glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vaoID);
        //std::cout << "Currently bound VAO: " << vaoID << std::endl;
        std::cout << "Uploading Mesh to GP" << std::endl;
        glBindVertexArray(0);

    }

    void Mesh::updateVertices(const std::vector<Vec3>& vertices)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0,
            vertices.size() * sizeof(Vec3),
            vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
            0,
            indices.size() * sizeof(unsigned int),
            indices.data());
    }

    void Mesh::DrawMesh() const {
        GLint vaoID;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vaoID);
        //std::cout << "Currently bound VAO: " << vaoID << std::endl;
        glBindVertexArray(VAO);
        if (!indices.empty()) {
            glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        else
            cout << "No Indices to draw!" << endl;
        glBindVertexArray(0);

    }