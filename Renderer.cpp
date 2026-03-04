#include "Renderer.h"

Renderer::Renderer(GLuint standardShader, GLuint curveShader)
    : m_StandardShader(standardShader),
	m_CurveShader(curveShader)
{
	glUseProgram(m_StandardShader);
	// Standard shader uniform locations
    m_MVPLocation = glGetUniformLocation(m_StandardShader, "MVP");
    m_ColorLocation = glGetUniformLocation(m_StandardShader, "uColor");

    // Curve shader uniform locations
	glUseProgram(m_CurveShader);
	m_ModelCurveLoc = glGetUniformLocation(m_CurveShader, "model");
    m_ViewCurveLoc = glGetUniformLocation(m_CurveShader, "view");
    m_ProjectionCurveLoc = glGetUniformLocation(m_CurveShader, "projection");

    // Optional: one-time curve uniforms
    glUniform1f(glGetUniformLocation(m_CurveShader, "segmentCount"), 40);
    glUniform1f(glGetUniformLocation(m_CurveShader, "stripCount"), 1);

}

void Renderer::Draw(
    const Mesh& mesh,
    const Transform& transform,
    const Camera& camera
    )
{
    glUseProgram(m_StandardShader);

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

void Renderer::DrawCurve(const CurveMesh& curveMesh, const Transform& transform, const Camera& camera, const glm::vec4& color)
{
    glUseProgram(m_CurveShader);


    // Set uniforms
    glm::mat4 model = transform.GetModelMatrix();
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = camera.GetProjectionMatrix();

    glUniformMatrix4fv(
        m_ModelCurveLoc,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    glUniformMatrix4fv(
        m_ViewCurveLoc,
        1,
        GL_FALSE,
        glm::value_ptr(view));

    glUniformMatrix4fv(
        m_ProjectionCurveLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection));

	glUniform4fv(
        m_ColorLocation,
        1,
        glm::value_ptr(color));

    // Required for Tessellation
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    // Draw the curve mesh
	curveMesh.Draw();
}
