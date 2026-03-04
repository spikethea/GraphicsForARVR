#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class CurveMesh
{
public:
    CurveMesh();
    ~CurveMesh();

    void init();
    void release();

    void build(const std::vector<glm::vec3>& controlPoints);
    void Draw() const;

    GLuint VAO = 0;
    GLuint VBO = 0;

private:
    std::vector<glm::vec3> m_controlPoints;
};