#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    float FOV = glm::radians(45.0f);
    float AspectRatio = 4.0f / 3.0f;
    float NearClip = 0.1f;
    float FarClip = 100.0f;

    glm::vec3 Position{ 0.0f, 0.0f, 3.0f };
    glm::vec2 Rotation{ 0.0f }; // x = yaw, y = pitch

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
};