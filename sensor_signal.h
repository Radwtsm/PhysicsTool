#ifndef SENSOR_SIGNAL_H
#define SENSOR_SIGNAL_H

#include <vector>

namespace SensorSim {
    // make a mock sinusoidal signal, simulating a smartphone accellerometer
    std::vector<float> generateMockBreathingData(int durationSeconds = 30, float amplitude=0.5f,float frequency = 0.25f, int samplingRate = 50);
}

#endif // SENSOR_SIGNAL_H
