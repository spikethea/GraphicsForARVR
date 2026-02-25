#pragma once


#include <vector>

struct Point {
	float x, y;
};

std::vector<Point> controlPoints = {
	{ -0.5f, -0.5f },
	{ -0.25f, 0.5f },
	{ 0.25f, -0.5f },
	{ 0.5f, 0.5f }
};

Point BezierPoint (float t) {
	float x = pow(1 - t, 3) * controlPoints[0].x + 3 * pow(1 - t, 2) * t * controlPoints[1].x + 3 * (1 - t) * pow(t, 2) * controlPoints[2].x + pow(t, 3) * controlPoints[3].x;
	float y = pow(1 - t, 3) * controlPoints[0].y + 3 * pow(1 - t, 2) * t * controlPoints[1].y + 3 * (1 - t) * pow(t, 2) * controlPoints[2].y + pow(t, 3) * controlPoints[3].y;
	return { x, y };
}
