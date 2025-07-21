//
// Created by ramesh on 7/7/2025.
//

#include "signal_simulator_window.h"
#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <d3d11.h>
#include <iostream>
#include <ostream>
#include <tchar.h>

#include <vector>
#include "../sensor_signal.h"
#include "../RealTimeSimulator.h"
#include <cmath>

#include "signal_simulator_window.h"
#include "../imgui/imgui.h"
#include "../implot/implot.h"
#include <d3d11.h>
#include <iostream>
#include <tchar.h>

#include <vector>
#include "../sensor_signal.h"
#include "../RealTimeSimulator.h"

namespace SensorSim
{
    // Buffer input per ImGui
    static char buf[128] = "";
    static bool show_error = false;
    static bool show_graph = false;

    // Simulatore tempo reale
    static RealTimeSimulator Simulator;

    // Vettori dati visualizzati
    static std::vector<float> data;       // valori respirazione
    static std::vector<float> times;      // tempi associati

    // Variabili controllo simulazione
    static float target_time = 0.0f;      // durata simulazione scelta dall'utente
    static float current_t = 0.0f;        // tempo simulato corrente
    static int samplingRate = 50;         // Hz
    static float dt = 1.0f / samplingRate;

    // Funzione per generare vettore tempi
    std::vector<float> times_vector(int samples)
    {
        std::vector<float> times;
        times.reserve(samples);
        for (int i = 0; i < samples; ++i)
            times.push_back(i * dt);
        return times;
    }

    void countRisesAndFalls(const std::vector<float>& data, int& rises, int& falls) {
        rises = 0;
        falls = 0;
        if (data.size() < 2) return;

        // Controlla salita iniziale
        if (data[0] < data[1]) {
            rises++;
        }
        else if (data[0] > data[1]) {
            falls++;
        }

        // Cerca minimi e massimi locali dal secondo al penultimo punto
        for (size_t i = 1; i + 1 < data.size(); ++i) {
            float prev = data[i - 1];
            float curr = data[i];
            float next = data[i + 1];

            if (curr < prev && curr < next) {
                rises++;  // minimo locale -> salita dopo
            }
            else if (curr > prev && curr > next) {
                falls++;  // massimo locale -> discesa dopo
            }
        }

        // (Opzionale) Controlla se c'è una salita o discesa all'ultimo punto
        // Se vuoi farlo, puoi aggiungerlo qui.
    }

    int countCompleteWaves(const std::vector<float>& data) {
        int rises = 0, falls = 0;
        countRisesAndFalls(data, rises, falls); // come ti ho mostrato prima
        // Una onda completa è una salita + una discesa, quindi minimo tra rises e falls
        return std::min(rises, falls);
    }


    // Funzione per generare dati mock fino a N campioni
    std::vector<float> generateMockDataSamples(int samples)
    {
        std::vector<float> values;
        values.reserve(samples);
        const float amplitude = 0.5f;
        const float frequency = 0.25f; // 15 respiri/min

        for (int i = 0; i < samples; ++i) {
            float t = i * dt;
            values.push_back(amplitude * std::sin(2 * M_PI * frequency * t));
        }
        return values;
    }

    void signal_simulator_window()
    {
        if (ImGui::Begin("Signal Simulator"))
        {

            ImGui::InputText("Duration", buf, IM_ARRAYSIZE(buf));



            if (ImGui::Button("Start Simulation"))
            {
                char* endptr;
                float val = strtof(buf, &endptr);

                if (buf[0] == '\0' || *endptr != '\0' || val <= 0.0f)
                {
                    show_error = true;
                    show_graph = false;
                }
                else
                {
                    show_error = false;
                    show_graph = true;

                    target_time = val;
                    current_t = 0.0f;

                    // Reset dati
                    data.clear();
                    times.clear();

                    // Avvio simulatore
                    Simulator.start();
                }
            }

            if (show_graph)
            {




                current_t = static_cast<float>(Simulator.elapsed());
if (current_t > target_time)
    current_t = target_time;

int samples_needed = static_cast<int>(current_t * samplingRate);
if (samples_needed > 0)
{
    data = generateMockDataSamples(samples_needed);
    times = times_vector(samples_needed);
}

// Conta quante onde complete (respiri)
int waves = countCompleteWaves(data);

float breaths_per_minute = 0.0f;
if (current_t > 0.0f) {
    breaths_per_minute = (waves / current_t) * 60.0f;  // respiri al minuto
}

                if (ImPlot::BeginPlot("Breathing Signal"))
                {
                    if (!data.empty())
                        ImPlot::PlotLine("Value(t)", times.data(), data.data(), (int)data.size());

                    ImPlot::EndPlot();
                }

                ImGui::Text("Elapsed time: %.2f / %.2f seconds", current_t, target_time);
                // Conta salite e discese
                int rises = 0, falls = 0;
                countRisesAndFalls(data, rises, falls);
                ImGui::Text("salite: %d / discese: %d", rises, falls);
                ImGui::Text("Frequenza respiro: %.3f Hz", breaths_per_minute);


            }


        }
        ImGui::End();
    }
}
