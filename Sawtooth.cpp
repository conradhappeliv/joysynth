#include "Sawtooth.h"

void Sawtooth::synthesize(std::vector<double>& out) {
    double samples = sampleRate/frequency;
    double delta = frequency/sampleRate*2;
    for(int i = 0; i < samples; i++) {
        curx += delta;
        if(curx > 1) curx -= 2;
        out.push_back(curx);
    }
}