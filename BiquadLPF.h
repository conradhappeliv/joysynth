//
// Created by conrad on 10/2/16.
//

#ifndef JOYSYNTH2_BIQUADLPF_H
#define JOYSYNTH2_BIQUADLPF_H

#include <cmath>
#include <vector>

class BiquadLPF {
public:
    BiquadLPF() { init(); }
    void process(std::vector<double>&);
    void setCutoff(double c) { cutoff = c/sampleRate; init(); }
private:
    void init();
    int sampleRate = 44100;
    double cutoff = 3000/44100.;
    double Q = 1.85;

    double a0, a1, a2, b1, b2;
    double z1 = 0, z2;
};


#endif //JOYSYNTH2_BIQUADLPF_H
