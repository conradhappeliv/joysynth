#include "ConvReverb.h"

using namespace std;

ConvReverb::ConvReverb() {
    ifstream f("/home/conrad/ClionProjects/joysynth2/wavtext");
    if(!f.is_open()) {
        cerr << "Couldn't open FIR file" << endl;
    }
    string tmp;
    while(f.good()) {
        f >> tmp;
        fir.push_back(stoi(tmp));
    }
    // normalize volume
    res_size = block_size*2 + fir.size() - 1;
    transform(fir.begin(), fir.end(), fir.begin(), bind1st(multiplies<double>(), 1./32768./res_size));

    // init vectors
    ffft_in.resize(block_size*2-1);
    ifft_in.resize(block_size*2-1);
    ffft_out.resize(block_size*2-1);
    ifft_out.resize(block_size*2-1);

    // create fft plans
    ffft_plan = fftw_plan_dft_r2c_1d(block_size*2-1, &ffft_in[0], (fftw_complex*) &ffft_out[0], FFTW_PATIENT);
    ifft_plan = fftw_plan_dft_c2r_1d(block_size*2-1, (fftw_complex*) &ifft_in[0], &ifft_out[0], FFTW_PATIENT);

    int sampsprocessed = 0;
    while(sampsprocessed < fir.size()) {
        int remaining = fir.size() - sampsprocessed;
        copy(fir.begin()+sampsprocessed, fir.begin()+sampsprocessed+min(block_size, remaining), ffft_in.begin());
        for(int i = 0; i < ffft_out.size(); i++) ffft_out[i] = complex<double>(0);
        fftw_execute(ffft_plan);
        FIR_segments.push_back(ffft_out);
        sampsprocessed += block_size;
    }

    processed.reserve(res_size);
}

ConvReverb::~ConvReverb() {
    fftw_destroy_plan(ifft_plan);
    fftw_destroy_plan(ffft_plan);
}

void ConvReverb::subprocess(const vector<double> in){
    processed.resize(res_size, 0);
    // fft of input vector
    copy(in.begin(), in.begin()+in.size(), ffft_in.begin());
    for(int i = in.size(); i < in.size()+block_size; i++) ffft_in[i] = 0;
    fftw_execute(ffft_plan);

    int sampsprocessed = 0;
    for(int i = 0; i < (int) FIR_segments.size(); i++) {
        const std::vector<std::complex<double>>& segment = FIR_segments[i];
        for(int i = 0; i < segment.size(); i++) {
            ifft_in[i] = segment[i] * ffft_out[i];
        }
        fftw_execute(ifft_plan);
        for(int i = 0; i < ifft_out.size(); i++) {
            processed[sampsprocessed+i] += ifft_out[i];
        }
        sampsprocessed += block_size;
    }

    for(int i = 0; i < in.size(); i++) processed[i] += in[i];
};