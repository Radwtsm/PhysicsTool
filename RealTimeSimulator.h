#pragma once

#ifndef REALTIMESIMULATOR_H
#define REALTIMESIMULATOR_H

class RealTimeSimulator {
private:
    double start_time = 0.0;
    double paused_elapsed = 0.0;
    bool running = false;

public:
    RealTimeSimulator();
    void start();
    double get_start();
    void pause();
    void resume();
    double elapsed() const;
};

#endif // REALTIMESIMULATOR_H
