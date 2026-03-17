#include "Renderer.h"

float bezier[16] = {
-1, 3,-3,1,
 3,-6, 3,0,
-3, 3, 0,0,
 1, 0, 0,0
};

Renderer::Renderer(GLuint standardShader, GLuint curveShader)
    : m_StandardShader(standardShader),
	m_CurveShader(curveShader)
{
	glUseProgram(m_StandardShader);
	// Standard shader uniform locations
    m_ModelLoc = glGetUniformLocation(m_StandardShader, "model");
    m_ViewLoc = glGetUniformLocation(m_StandardShader, "view");
    m_ProjectionLoc = glGetUniformLocation(m_StandardShader, "projection");

    std::cout << "Model loc: " << m_ModelLoc << std::endl;
    std::cout << "View loc: " << m_ViewLoc << std::endl;
    std::cout << "Projection loc: " << m_ProjectionLoc << std::endl;

    // Curve shader uniform locations
	glUseProgram(m_CurveShader);
	m_ModelCurveLoc = glGetUniformLocation(m_CurveShader, "model");
    m_ViewCurveLoc = glGetUniformLocation(m_CurveShader, "view");
    m_ProjectionCurveLoc = glGetUniformLocation(m_CurveShader, "projection");
    m_ColorCurveLoc = glGetUniformLocation(m_CurveShader, "uColor");
    // Curve Shader Basis uniform locations
    basisLoc = glGetUniformLocation(m_CurveShader, "basis");
	// Tessellation level uniform location
	segmentLoc = glGetUniformLocation(m_CurveShader, "segmentCount");

    if (basisLoc == -1)
        std::cout << "basis uniform not found\n";

    // Optional: one-time curve uniforms
    glUniform1f(glGetUniformLocation(m_CurveShader, "stripCount"), 1);

    std::cout << "Model loc: " << m_ModelCurveLoc << std::endl;
    std::cout << "View loc: " << m_ViewCurveLoc << std::endl;
    std::cout << "Projection loc: " << m_ProjectionCurveLoc << std::endl;
    std::cout << "Color loc: " << m_ColorCurveLoc << std::endl;
    std::cout << "Basis loc: " << basisLoc << std::endl;
	std::cout << "Segment loc: " << segmentLoc << std::endl;

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


    glUniformMatrix4fv(
        m_ModelLoc,
        1,
        GL_FALSE,
        glm::value_ptr(model));

    glUniformMatrix4fv(
        m_ViewLoc,
        1,
        GL_FALSE,
        glm::value_ptr(view));

    glUniformMatrix4fv(
        m_ProjectionLoc,
        1,
        GL_FALSE,
        glm::value_ptr(projection));

    //drawing the mesh
    mesh.DrawMesh();
}

void Renderer::DrawCurve(const CurveMesh& curveMesh, const Transform& transform, const Camera& camera, const glm::vec4& color, const float curve[16])
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

    glUniformMatrix4fv(
        basisLoc,
        1,
        GL_FALSE,
        curve
    );

	glUniform4fv(
        m_ColorCurveLoc,
        1,
        glm::value_ptr(color));

    glUniform1f(segmentLoc, segmentCount);

    // Required for Tessellation
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    // Draw the curve mesh
	curveMesh.Draw();
}
