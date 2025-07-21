#include "sensor_signal.h"
#include <cmath> // per sin() e M_PI

namespace SensorSim {

    std::vector<float> generateMockBreathingData(int durationSeconds, float amplitude,float frequency, int samplingRate) {
        std::vector<float> data;

        const int totalSamples = durationSeconds * samplingRate;

        for (int i = 0; i < totalSamples; ++i) {
            float t = static_cast<float>(i) / samplingRate;
            float value = amplitude * std::sin(2 * M_PI * frequency * t);
            data.push_back(value);
        }

        return data;
    }

} // namespace SensorSim
