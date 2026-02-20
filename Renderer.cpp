#include "Renderer.h"

Renderer::Renderer(GLuint shader)
    : m_Shader(shader)
{
    m_MVPLocation = glGetUniformLocation(shader, "MVP");
    m_ColorLocation = glGetUniformLocation(m_Shader, "uColor");
}

void Renderer::Draw(
    const Mesh& mesh,
    const Transform& transform,
    const Camera& camera
    )
{
    //MVP Matrix
    glm::mat4 model = transform.GetModelMatrix();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    glm::mat4 mvp = projection * view * model;



    glUniformMatrix4fv(
        m_MVPLocation,
        1,
        GL_FALSE,
        glm::value_ptr(mvp)
    );

    //drawing the mesh
    mesh.DrawMesh();
}