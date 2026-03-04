#include "CurveMesh.h"
#include <iostream>

CurveMesh::CurveMesh()
{
    init();
}

CurveMesh::~CurveMesh()
{
    release();
}

void CurveMesh::init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    std::cout << "CurveMesh created: VAO=" << VAO
        << ", VBO=" << VBO << std::endl;
}

void CurveMesh::release()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    std::cout << "CurveMesh destroyed: VAO=" << VAO << std::endl;
}

void CurveMesh::build(const std::vector<glm::vec3>& controlPoints)
{
    if (controlPoints.size() != 4)
    {
        std::cerr << "Bezier patch requires exactly 4 control points!"
            << std::endl;
        return;
    }

    m_controlPoints = controlPoints;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        m_controlPoints.size() * sizeof(glm::vec3),
        m_controlPoints.data(),
        GL_STATIC_DRAW);

    // layout(location = 0) vec3 position;
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    std::cout << "CurveMesh uploaded with 4 control points." << std::endl;
}

void CurveMesh::Draw() const
{
    // Tell OpenGL how many vertices per patch
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glBindVertexArray(VAO);

    glDrawArrays(GL_PATCHES, 0, 4);

    glBindVertexArray(0);
}