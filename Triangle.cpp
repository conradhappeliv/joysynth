#include "Triangle.h"

void Triangle::synthesize(std::vector<double>& out) {
    double samples = sampleRate/frequency;
    for(int i = 0; i < samples/2; i++) out.push_back(i*2/samples*2 - 1);
    for(int i = (int) ceil(samples/2); i < samples; i++) out.push_back(-1*((i-samples/2)*2/samples*2 - 1));
}