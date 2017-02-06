#include "Triangle.h"

void Triangle::synthesize(std::vector<double>& out) {
    double samples = sampleRate/frequency;
    double delta = frequency/sampleRate*4;
    for(int i = 0; i < samples; i++){
        curx += pos*delta;
        if(abs(curx) > 1) {
            curx = 2*pos-curx;
            pos *= -1;
        }
        out.push_back(curx);
    }
}