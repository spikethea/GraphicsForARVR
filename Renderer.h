#pragma once

#include <glad/glad.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "Camera.h"
#include "Transform.h"

#include "Mesh.h"
#include "curveMesh.h"

class Renderer
{
public:
    Renderer(GLuint shader, GLuint curveShader);


    void Draw(
    const Mesh& mesh,
    const Transform& transform,
    const Camera& camera
    );

    void DrawCurve(
        const CurveMesh& curveMesh,
        const Transform& transform,
        const Camera& camera,
        const glm::vec4& color
    );

private:
    GLuint m_StandardShader;
    GLuint m_CurveShader;

    

    GLint m_MVPLocation;
	GLint m_ColorLocation;

    // curve shader locations
    GLint m_ModelCurveLoc;
    GLint m_ViewCurveLoc;
    GLint m_ProjectionCurveLoc;
};