#include <iostream>
#include "BiquadLPF.h"

using namespace std;

void BiquadLPF::init() {
    double K = tan(M_PI*cutoff);
    double norm = 1/(1+K/Q+K*K);

    a0 = K*K*norm;
    a1 = 2*a0;
    a2 = a0;
    b1 = 2*(K*K-1)*norm;
    b2 = (1-K/Q+K*K)*norm;
}

void BiquadLPF::subprocess(const RTArray<double> &input) {
    for(int i = 0; i < input.size(); i++) {
        double old = input[i];
        processed[i] = old*a0+z1;
        z1 = old*a1 + z2 - b1*(processed[i]);
        z2 = old*a2 - b2*(processed[i]);
    }
}