#include "Plotter.h"

void Plotter::init() {
    in = new double[fft_size];
    out = new double[fft_size];
    for(int i = 0; i < fft_size; i++)
        in[i] = out[i] = 0;

    plan = fftw_plan_r2r_1d(fft_size, in, out, FFTW_REDFT00, FFTW_ESTIMATE);
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

void Plotter::plot(const std::vector<double> data) {
    std::copy(in+data.size(), in+fft_size, in); // shift input array left
    std::copy(data.cbegin(), data.cend(), in+fft_size-data.size()); // new data -> input array
    fftw_execute(plan);

    std::vector<std::pair<double, double>> outdata;
    for(int i = 0; i < 2000; i++)
        outdata.push_back(std::make_pair(i / 2. * sample_rate / fft_size, out[i]/sample_rate));

    // time
    if(first_plot) {
        gp << "set term x11 0\n";
        gp << "plot '-' with impulses\n";
        gp.send1d(std::vector<double>(in+fft_size-data.size(), in+fft_size));

        // frequency
        gp << "set term x11 1\n";
        gp << "set yrange [-1:1]\n";
        gp << "plot '-' with impulses\n";
        gp.send1d(outdata);
        first_plot = false;
    } else {
        gp << "set term x11 0\n";
        gp << "replot\n";
        gp.send1d(std::vector<double>(in+fft_size-data.size(), in+fft_size));
        gp << "set term x11 1\n";
        gp << "replot\n";
        gp.send1d(outdata);
    }

}

Plotter::~Plotter() {
    delete[] in;
    delete[] out;
    fftw_destroy_plan(plan);
}