#pragma once

#include <vector>
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "PalmTree.h"


class GUI {
public:
	float size = 0.05f;

	void init(GLFWwindow* window);

	void draw(PalmTree& palmTree, Renderer& renderer);

	void UIrender();

	void cleanup();
};