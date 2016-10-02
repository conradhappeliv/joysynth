#include "Triangle.h"

void Triangle::synthesize(std::vector<double>& out) {
    double samples = sampleRate/frequency;
    for(int i = 0; i < samples/2; i++) out.push_back((i-phase/2/M_PI)*2/samples*2 - 1);
    for(int i = (int) ceil(samples/2); i < samples; i++) out.push_back(-1*(((i-phase/2/M_PI)-samples/2)*2/samples*2 - 1));
    phase += (1 - ((int)samples)/samples)*2*M_PI;
}