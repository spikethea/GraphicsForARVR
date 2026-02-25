#include "GUI.h"

#include "Mesh.h"

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

void GUI::draw(int& activeIndex, std::vector<Mesh>& meshes) {
    // shorten deeply nested variable
    //auto& iterations = trees[activeIndex].lSystem.iterations;

    //IMGUI RENDER
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGui window
    {
        ImGui::Begin("Menu");                          // Create a window called "Hello, ImGui!" and append into it.
        ImGui::Text("L System");               // Display some text (you can use a format strings too)
        ImGui::Text("Scale");
        /*if (ImGui::SliderFloat("size", &trees[activeIndex].unit, 0.005, 0.2)) {
            trees[activeIndex].updateLSystem();
        };*/

        ImGui::Text("Set Angle (Degrees)");
        //if (ImGui::SliderFloat("angle", &trees[activeIndex].angleDeg, 0, 90)) {
        //    //trees[activeIndex].updateLSystem();
        //};
        
        ImGui::Text("How many Iterations?");
        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
            //if (iterations > 0) {
            //    iterations--;
            //    //trees[activeIndex].updateLSystem();
            //}

        }

        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
            //if (iterations < 7) {
            //    iterations++;
            //    //trees[activeIndex].updateLSystem();
            //}
            
        }
        ImGui::PopItemFlag();
        ImGui::SameLine();
        //ImGui::Text("%d", iterations);

        ImGui::Text("Edge-Rewriting");
        for (int i = 0; i < 6; i++)
        {
            if (i > 0)
                ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
            if (ImGui::Button("Click")) {
				activeIndex = i;
            };
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }

        ImGui::Text("Stochastic");
        if (ImGui::Button("Fuzzy Weed")) {
            activeIndex = 6;
            //trees[activeIndex].updateLSystem();
        };
        ImGui::SetItemTooltip("Re-Click to generate random values");
        ImGui::SameLine();
        if (ImGui::Button("Binary Tree")) {
            activeIndex = 7;
            //trees[activeIndex].updateLSystem();
        };
        ImGui::SetItemTooltip("Re-Click to generate random values");
        ImGui::Separator();
        ImGui::Text("Tip: You can also Hotkey through the L-Systems");
        ImGui::Text("and adjust iterations with WASD/Arrow Keys");

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