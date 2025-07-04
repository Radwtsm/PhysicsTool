#include "RealTimeSimulator.h"
#include "../imgui/imgui.h"

RealTimeSimulator::RealTimeSimulator()
    : start_time(0.0), paused_elapsed(0.0), running(false) {}

void RealTimeSimulator::start() {
    start_time = ImGui::GetTime();
    paused_elapsed = 0.0;
    running = true;
}

double RealTimeSimulator::get_start() {
    return start_time;
}

void RealTimeSimulator::pause() {
    if (running) {
        paused_elapsed = elapsed();
        running = false;
    }
}

void RealTimeSimulator::resume() {
    if (!running) {
        start_time = ImGui::GetTime() - paused_elapsed;
        running = true;
    }
}

double RealTimeSimulator::elapsed() const {
    if (running) {
        return ImGui::GetTime() - start_time;
    }
    return paused_elapsed;
}
