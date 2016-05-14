#include "Square.h"

void Square::synthesize(std::vector<double>& out) {
    double freq = 2.*M_PI*frequency/sampleRate;
    for(int i = 0; i < sampleRate/frequency; i++) {
        float val = cos(i*freq);
        if(val >= 0)
            out.push_back(1);
        else
            out.push_back(-1);
    }
}