//
// Created by ramesh on 7/4/2025.
//
#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <d3d11.h>
#include <tchar.h>
#include "frefall_window.h"
#include <vector>
#include "../RealTimeSimulator.h"

// user seconds input text
char buf[128] = "";

bool show_error = false;
bool show_graph = false;

std::vector<double> x_data;
std::vector<double> y_data;

// initial zoom levels
double x_min = 0.0, x_max = 5.0;  // initial X
double y_min = 0.0, y_max = 50.0; // initial Y

double target_time = 0.0;
double current_t = 0.0;
float delta_t = 0.1;
float Xo = 0;
float Vo = 0;
float a = 9.8;

double start_time = 0.0;
auto Simulator = RealTimeSimulator();

// add values for the current t in x_data(time), y_data (position)
void update_freefall_simulation(const double current_t) {
    if (current_t <= target_time) {
        const double x = Xo + (Vo * current_t) + 0.5 * a * current_t * current_t;

        if (x_data.empty() || current_t > x_data.back()) {
            x_data.push_back(current_t);
            y_data.push_back(x);
        }
    }

    if (current_t >= target_time && (x_data.empty() || x_data.back() < target_time)) {
        const double x_final = Xo + (Vo * target_time) + 0.5 * a * target_time * target_time;
        x_data.push_back(target_time);
        y_data.push_back(x_final);
    }
}

// TODO Add another field, where the user can insert a distance (m), and see how much it takes to travel that distance (button to stop is needed).
// Shows distance traveled by a freefalling object. The graph shows position over time.
void freefall_window() {

    // initialize the simulation window
    if (ImGui::Begin("Free Fall Simulation")) {
        ImGui::Text("Enter simulation time (seconds):");
        ImGui::InputText("Time (seconds)", buf, IM_ARRAYSIZE(buf));

        if (show_error) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not a valid float!");
        }

        if (ImGui::Button("Run Free Fall Simulation")) {
            char* endptr;

            // takes the value from the input widget.
            // checks if the string is empty
            // checks that there are no letters
            // checks for negative values
            // gets the string in buf and casts it to float
            if (const float value = strtof(buf, &endptr); buf[0] == '\0' || *endptr != '\0' || value <= 0.0f) {
                show_error = true;
                show_graph = false;
            } else {
                // starts simulation with correct data
                show_error = false;
                show_graph = true;

                // float from user input to float
                target_time = value;
                current_t = 0.0;
                x_data.clear();
                y_data.clear();

                Simulator.start();
                start_time = Simulator.get_start();
            }
        }

        if (show_graph) {
            // now() - start_time
            const double elapsed_time = Simulator.elapsed();
            current_t = elapsed_time;

            if (current_t <= target_time) {
                update_freefall_simulation(current_t);
            }

            // buttons for zooming in and out.
            if (ImGui::Button("Zoom In")) {
                double x_range = (x_max - x_min) * 0.5;
                double y_range = (y_max - y_min) * 0.5;
                x_min += x_range * 0.25;
                x_max -= x_range * 0.25;
                y_min += y_range * 0.25;
                y_max -= y_range * 0.25;
            }

            ImGui::SameLine();

            if (ImGui::Button("Zoom Out")) {
                double x_range = (x_max - x_min);
                double y_range = (y_max - y_min);
                x_min -= x_range * 0.5;
                x_max += x_range * 0.5;
                y_min -= y_range * 0.5;
                y_max += y_range * 0.5;
            }


            if (ImPlot::BeginPlot("Position vs Time")) {
                // default starting limits for the axis.
                ImPlot::SetupAxisLimits(ImAxis_X1, x_min, x_max, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, y_min, y_max, ImGuiCond_Always);

                ImPlot::PlotLine("x(t)", x_data.data(), y_data.data(),   x_data.size());
                ImPlot::EndPlot();

                if (!y_data.empty()) {
                    ImGui::Text("%.2f m traveled in %.2f seconds", y_data.back(), x_data.back());
                }
            }
        }
    }
    ImGui::End();
}
