#include "BezierEvaluation.h"

glm::vec3 EvaluateBezier(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    float t)
{
    float u = 1.0f - t;

    float b0 = u * u * u;
    float b1 = 3.0f * u * u * t;
    float b2 = 3.0f * u * t * t;
    float b3 = t * t * t;

    return b0 * P0 +
        b1 * P1 +
        b2 * P2 +
        b3 * P3;
}

glm::vec3 BezierTangent(
    const glm::vec3& P0,
    const glm::vec3& P1,
    const glm::vec3& P2,
    const glm::vec3& P3,
    float t)
{
    float u = 1.0f - t;

    return
        3.0f * u * u * (P1 - P0) +
        6.0f * u * t * (P2 - P1) +
        3.0f * t * t * (P3 - P2);
}