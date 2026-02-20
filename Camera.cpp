#include "Camera.h"

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(FOV, AspectRatio, NearClip, FarClip);
}

glm::mat4 Camera::GetViewMatrix() const
{
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::translate(view, -Position);
    view = glm::rotate(view, Rotation.y, glm::vec3(1, 0, 0));
    view = glm::rotate(view, Rotation.x, glm::vec3(0, 1, 0));

    return view;
}