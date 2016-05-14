#ifndef JOYSYNTH2_PLOTTER_H
#define JOYSYNTH2_PLOTTER_H

#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

#include "gnuplot-iostream.h"
#include "fftw3.h"

class Plotter {
private:
    Gnuplot* gp; // hacky way to get around a private operator= (??)
    fftw_plan plan;
    int sample_rate;
    int fft_size;
    double* in;
    fftw_complex* out;
    bool first_plot = true;

    void init();
public:
    Plotter();
    Plotter(int, int);
    void plot(const std::vector<double>, bool, bool);
    ~Plotter();
};


#endif //JOYSYNTH2_PLOTTER_H
