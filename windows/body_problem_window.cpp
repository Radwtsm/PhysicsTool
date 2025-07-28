//
// Created by ramesh on 7/24/2025.
//

#include "body_problem_window.h"

//
// Created by ramesh on 7/4/2025.
//
#include "../imgui/imgui.h"

void n_body_problem_window() {
    if (ImGui::Begin("N-BODY PROBLEM")) {
        ImGui::Text("N-body simulation placeholder");

        // You can add sliders, input boxes, plots, etc. here
        // Example UI placeholder
        ImGui::Text("Add your simulation controls here.");
        ImGui::Button("Start Simulation");
    }
    ImGui::End();
}
