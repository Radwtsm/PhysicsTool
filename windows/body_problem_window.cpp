#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <map>
#include "../RealTimeSimulator.h" // Your simulator class
#include <vector>
#include <iostream>

using namespace std;

class Particle {
public:
    double massa, posizione_x, posizione_y, velocita_x, velocita_y;

    void set_massa(float n_massa) {
        this->massa = n_massa;
    }

    void set_posizione(float n_posizione_x, float n_posizione_y) {
        this->posizione_x = n_posizione_x;
        this->posizione_y = n_posizione_y;
    }

    void set_velocita(float n_velocita_x, float n_velocita_y) {
        this->velocita_x = n_velocita_x;
        this->velocita_y = n_velocita_y;
    }
};

class Container {
public:
    std::vector<Particle> particles;

    void add_particle(const Particle& p) {
        particles.push_back(p);
    }

    void clear_particles() {
        particles.clear();
    }

    size_t size() const {
        return particles.size();
    }

    void update()
    {
        std::cout << particles.size() << std::endl;
    }

    Particle& operator[](size_t index) {
        return particles[index];
    }
};

extern Container contenitore;
extern double delta_t;

static bool running = false;
static RealTimeSimulator simulator;

// Stores trajectories
std::map<int, std::vector<double>> history_x;
std::map<int, std::vector<double>> history_y;

// Track simulation time
static double last_sim_time = 0.0;

void reset_simulation() {
    running = false;
    simulator = RealTimeSimulator();
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
        }
    }
}

void particle_simulation_window() {
    if (ImGui::Begin("Particle Gravity Simulation (Real Time)")) {

        if (ImGui::Button("Start Simulation")) {
            running = true;
            simulator.start();
            last_sim_time = 0.0;
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            reset_simulation();
        }

        ImGui::SliderFloat("Delta t", reinterpret_cast<float*>(&delta_t), 0.0001f, 0.1f, "%.4f");

        if (running) {
            advance_simulation_real_time();

            if (ImPlot::BeginPlot("Particle Trajectories", ImVec2(-1, 400))) {
                ImPlot::SetupAxes("X [m]", "Y [m]", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

                for (int i = 0; i < contenitore.size(); ++i) {
                    std::string label = "Particle " + std::to_string(i);
                    ImPlot::PlotLine(label.c_str(), history_x[i].data(), history_y[i].data(), history_x[i].size());
                }

                ImPlot::EndPlot();
            }

            for (int i = 0; i < contenitore.size(); ++i) {
                auto& p = contenitore[i];
                ImGui::Text("P%d: Pos(%.2f, %.2f), Vel(%.2f, %.2f)", i, p.posizione_x, p.posizione_y, p.velocita_x, p.velocita_y);
            }

            ImGui::Text("Elapsed Simulation Time: %.2f seconds", last_sim_time);
        }
    }
    ImGui::End();
}
