#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <map>

extern Container contenitore; // dichiarazione esterna del tuo contenitore
extern double delta_t;
static bool show_particle_sim = false;

// memorizza la storia delle posizioni per ogni particella (per plot)
std::map<int, std::vector<double>> history_x;
std::map<int, std::vector<double>> history_y;

void update_particle_simulation() {
    contenitore.update();
    for (int i = 0; i < contenitore.size(); ++i) {
        history_x[i].push_back(contenitore[i].posizione_x);
        history_y[i].push_back(contenitore[i].posizione_y);
    }
}

void particle_simulation_window() {
    if (ImGui::Begin("Particle Gravity Simulation")) {
        if (ImGui::Button("Start Simulation")) {
            show_particle_sim = true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            show_particle_sim = false;
            history_x.clear();
            history_y.clear();
        }

        if (show_particle_sim) {
            update_particle_simulation();

            if (ImPlot::BeginPlot("Particle Trajectories")) {
                ImPlot::SetupAxes("X [m]", "Y [m]", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

                for (int i = 0; i < contenitore.size(); ++i) {
                    std::string label = "Particle " + std::to_string(i);
                    ImPlot::PlotLine(label.c_str(), history_x[i].data(), history_y[i].data(), history_x[i].size());
                }

                ImPlot::EndPlot();
            }

            // Stampa posizione e velocità in tempo reale
            for (int i = 0; i < contenitore.size(); ++i) {
                auto& p = contenitore[i];
                ImGui::Text("Particle %d: Pos(%.2f, %.2f) Vel(%.2f, %.2f)", i, p.posizione_x, p.posizione_y, p.velocita_x, p.velocita_y);
            }
        }
    }
    ImGui::End();
}
