#include "Plotter.h"

using namespace std;

void Plotter::init() {
    gp = new Gnuplot();

    in = new double[fft_size];
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*fft_size);
    for(int i = 0; i < fft_size; i++) { in[i] = 0; }
    plan = fftw_plan_dft_r2c_1d(fft_size, in, out, FFTW_ESTIMATE);
}

Plotter::Plotter() { // default sample rate & FFT size
    sample_rate = 44100;
    fft_size = 32768;
    init();
}

Plotter::Plotter(int samp_rate, int FFT_s) {
    sample_rate = samp_rate;
    fft_size = FFT_s;
    init();
}

void Plotter::add_data(const std::vector<double> data) {
    std::copy(in+data.size(), in+fft_size, in); // shift input array left
    std::copy(data.cbegin(), data.cend(), in+fft_size-data.size()); // new data -> input array
    last_add_size = data.size();
}

void Plotter::plot(bool timeplot, bool freqplot) {
    fftw_execute(plan);

    std::vector<std::pair<double, double>> outdata;
    for(int i = 0; i < 3500; i++)
        outdata.push_back(std::make_pair(i * sample_rate / fft_size,
                                         10*sqrt(pow(out[i][0], 2) + pow(out[i][1],2))/sample_rate));

    std::vector<double> timedata(in+fft_size-last_add_size, in+fft_size);
    // time
    if(first_plot) {
        if(timeplot) {
            *gp << "set term x11 0\n";
            *gp << "plot '-' with lines\n";
            gp->send1d(timedata);
        }

        if(freqplot) {
            *gp << "set term x11 1\n";
            *gp << "set yrange [-1:1]\n";
            *gp << "plot '-' with lines\n";
            gp->send1d(outdata);
        }
        first_plot = false;
    } else {
        if(timeplot) {
            *gp << "set term x11 0\n";
            *gp << "replot\n";
            gp->send1d(timedata);
        }
        if(freqplot) {
            *gp << "set term x11 1\n";
            *gp << "replot\n";
            gp->send1d(outdata);
        }
    }

}

Plotter::~Plotter() {
    delete[] in;
    delete[] out;

    delete gp;

    fftw_destroy_plan(plan);
}