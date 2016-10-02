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

void BiquadLPF::process(std::vector<double>& buffer) {
    for(auto it = buffer.begin(); it != buffer.end(); it++) {
        double old = *it;
        *it = old*a0+z1;
        z1 = old*a1 + z2 - b1*(*it);
        z2 = old*a2 - b2*(*it);
    }
}