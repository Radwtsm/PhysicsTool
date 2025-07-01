// 07/01/2025

#include <iostream>


using namespace std;

// Xo posizione iniziale
// Vo velocità iniziale
// t tempo
// accellerazione (costante)
int free_falling_without_air_resistance(float t, float delta_t = 0.1, float Xo = 0, float Vo = 0, float a = 9.80) {
    // freefalling object
    float current_t = 0;

    while (current_t<t) {
        double x = Xo + (Vo * t) + 0.5 * a * current_t * current_t;

        cout << "t = " << current_t << " secondi  x  = " << x << "m" << endl;
        current_t += delta_t;
    }
    return 0;
};

