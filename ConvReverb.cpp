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

    processed = new double[fir.size() + 1024 - 1];
    processedsize = fir.size() + 1024 - 1;
}

ConvReverb::~ConvReverb() {
    fftw_destroy_plan(ifft_plan);
    fftw_destroy_plan(ffft_plan);
}

void ConvReverb::subprocess(const RTArray<double>& in){
    size_t size = in.size();
    // fft of input vector
    for(size_t i = 0; i < size; i++) ffft_in[i] = in[i];
    for(int i = size; i < size+block_size-1; i++) ffft_in[i] = 0;
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
    for(int i = 0; i < processedsize; i++) processed[i] *= amount;
    for(int i = 0; i < size; i++) processed[i] += in[i];
};