#pragma once

#include <vector>
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Mesh.h"


class GUI {
public:
	float size = 0.05f;

	void init(GLFWwindow* window);

	void draw(int& activeIndex, std::vector<Mesh>& trees);

	void UIrender();

	void cleanup();
};