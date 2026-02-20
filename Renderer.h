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

class Renderer
{
public:
    Renderer(GLuint shader);


    void Draw(
    const Mesh& mesh,
    const Transform& transform,
    const Camera& camera
    );

private:
    GLuint m_Shader = 0;

    glm::mat4 m_Projection = glm::mat4(1.0f);
    glm::mat4 m_View = glm::mat4(1.0f);

    GLint m_MVPLocation = -1;
    GLint m_ColorLocation = -1;
};