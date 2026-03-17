#include "GUI.h"

void GUI::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);

    ImGui::GetStyle().ScaleAllSizes(xscale);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.FontGlobalScale = xscale;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::draw(PalmTree &palmTree, Renderer &renderer) {
    // shorten deeply nested variable
    //auto& iterations = trees[activeIndex].lSystem.iterations;

    //IMGUI RENDER
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGui window
    {
        ImGui::Begin("Menu");                          // Create a window called "Hello, ImGui!" and append into it.
        ImGui::Text("Curves");               // Display some text (you can use a format strings too)
        ImGui::Text("LeafScale");
        if (ImGui::SliderFloat("radius", &palmTree.radius, 0.005, 2)) {
			palmTree.updatePalmTree();
        };

        if (ImGui::SliderInt("Number Of Leaves", &palmTree.numLeaves, 3, 12)) {
            palmTree.updatePalmTree();
        };

        if (ImGui::SliderInt("Number Of Leaflet", &palmTree.numLeaflets, 5, 25)) {
            palmTree.updatePalmTree();
        };

        ImGui::Text("Tessellation");

        ImGui::SliderFloat("Segments", &renderer.segmentCount, 1.0f, 16.0f);
        
        ImGui::End();
    }
}

void GUI::UIrender() {
    //IMGUI RENDER
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::cleanup() {
    //IMGUI CLEANUP
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}