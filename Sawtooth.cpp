#include "Sawtooth.h"

void Sawtooth::synthesize(std::vector<double>& out) {
    double samples = sampleRate/frequency;
    for(int i = 0; i < samples; i++) out.push_back(i/samples*2 - 1);
}