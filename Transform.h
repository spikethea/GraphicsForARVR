#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    glm::vec3 Position{ 0.0f };
    glm::vec3 Rotation{ 0.0f }; // in radians
    glm::vec3 Scale{ 1.0f };

    glm::mat4 GetModelMatrix() const;
};
