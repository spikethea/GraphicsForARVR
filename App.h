#pragma once

#include <iostream>
#pragma once

#include "GUI.h"
#include "Mesh.h"



class App {
public:
    std::vector<Mesh> meshes;
    int activeIndex = 0;

	GUI gui;

    void init(GLFWwindow* window);

    void release();


    void onKey(int key, int scancode, int action, int mods);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void selectActiveLSystem(int index);

    void updateActiveLSystem(string Lsystem, float unitLength, float angleDeg);
};