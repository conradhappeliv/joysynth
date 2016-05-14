#include "Sine.h"

void Sine::synthesize(std::vector<double>& out) {
    double freq = 2.*M_PI*frequency/sampleRate;
    for(int i = 0; i < sampleRate/frequency; i++) {
        out.push_back(cos(i*freq));
    }
}