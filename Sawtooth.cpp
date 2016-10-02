#include "Sawtooth.h"

void Sawtooth::synthesize(std::vector<double>& out) {
    double samples = sampleRate/frequency;
    for(int i = 0; i < samples; i++) out.push_back((i-phase/2/M_PI)/samples*2 - 1);
    phase += (1-((int) samples)/samples)*2*M_PI;
}