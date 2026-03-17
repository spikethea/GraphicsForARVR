#pragma once
#include <glm/ext/vector_float3.hpp>

glm::vec3 EvaluateBezier(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    float t);

glm::vec3 BezierTangent(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    float t);
