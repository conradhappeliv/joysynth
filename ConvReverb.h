#ifndef JOYSYNTH2_CONVREVERB_H
#define JOYSYNTH2_CONVREVERB_H

#include "Effect.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <complex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>

#include "fftw3.h"

class ConvReverb : public Effect {
public:
    ConvReverb();
    ~ConvReverb();
protected:
    virtual void subprocess(const std::vector<double>);
private:
    const int block_size = 1024;
    int res_size;
    std::vector<double> fir;
    std::vector<std::vector<std::complex<double>>> FIR_segments;
    std::vector<double> ffft_in, ifft_out;
    std::vector<std::complex<double>> ffft_out, ifft_in;
    fftw_plan ffft_plan, ifft_plan;
};


#endif //JOYSYNTH2_CONVREVERB_H
