#ifndef JOYSYNTH2_PLOTTER_H
#define JOYSYNTH2_PLOTTER_H

#include <vector>
#include <algorithm>
#include <utility>

#include "gnuplot-iostream.h"
#include "fftw3.h"

class Plotter {
private:
    Gnuplot gp;
    fftw_plan plan;
    int sample_rate;
    int fft_size;
    double* in;
    double* out;
    bool first_plot = true;

    void init();
public:
    Plotter();
    Plotter(int, int);
    void plot(const std::vector<double>);
    ~Plotter();
};


#endif //JOYSYNTH2_PLOTTER_H
