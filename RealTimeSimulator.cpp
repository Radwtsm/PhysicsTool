//
// Created by ramesh on 7/28/2025.
//

#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <map>
#include <string>
#include "../RealTimeSimulator.h"
// #include "../Container.h"  // Assicurati che contenga la definizione di `extern Container contenitore`

extern Container contenitore;
extern double delta_t;

static bool running = false;
static bool paused = false;
static RealTimeSimulator simulator;

// Salva traiettorie
static std::map<int, std::vector<double>> history_x;
static std::map<int, std::vector<double>> history_y;

// Tempo simulato fino all'ultimo step
static double last_sim_time = 0.0;

void reset_simulation() {
    running = false;
    paused = false;
    simulator = RealTimeSimulator(); // resetta start time
    last_sim_time = 0.0;
    history_x.clear();
    history_y.clear();
}

void advance_simulation_real_time() {
    double now = simulator.elapsed();
    while (last_sim_time + delta_t < now) {
        contenitore.update();
        last_sim_time += delta_t;

        for (int i = 0; i < contenitore.size(); ++i) {
            history_x[i].push_back(contenitore[i].posizione_x);
            history_y[i].push_back(contenitore[i].posizione_y);

            // limita la lunghezza delle traiettorie per evitare overflow
            const size_t max_points = 10000;
            if (history_x[i].size() > max_points) {
                history_x[i].erase(history_x[i].begin());
                history_y[i].erase(history_y[i].begin());
            }
        }
    }
}

void particle_simulation_window() {
    if (ImGui::Begin("Particle Gravity Simulation (Real Time)")) {

        if (!running && ImGui::Button("Start Simulation")) {
            simulator.start();
            last_sim_time = 0.0;
            running = true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            reset_simulation();
        }

        ImGui::SliderFloat("Delta t", reinterpret_cast<float*>(&delta_t), 0.0001f, 0.1f, "%.4f");

        if (running) {
            if (ImGui::Button(paused ? "Resume" : "Pause")) {
                paused = !paused;
                if (!paused) {
                    simulator.resume();  // aggiorna il tempo di partenza se servisse
                }
            }

            if (!paused) {
                advance_simulation_real_time();
            }

            // Mostra grafico delle traiettorie
            if (ImPlot::BeginPlot("Trajectories", ImVec2(-1, 400))) {
                ImPlot::SetupAxes("X [m]", "Y [m]", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

                for (int i = 0; i < contenitore.size(); ++i) {
                    std::string label = "Particle " + std::to_string(i);
                    ImPlot::PlotLine(label.c_str(), history_x[i].data(), history_y[i].data(), history_x[i].size());
                }

                ImPlot::EndPlot();
            }

            // Mostra valori delle particelle in tempo reale
            for (int i = 0; i < contenitore.size(); ++i) {
                const auto& p = contenitore[i];
                ImGui::Text("P%d: Pos(%.2f, %.2f) Vel(%.2f, %.2f)",
                            i, p.posizione_x, p.posizione_y, p.velocita_x, p.velocita_y);
            }

            ImGui::Text("Elapsed simulation time: %.2f seconds", last_sim_time);
        }
    }

    ImGui::End();
}
