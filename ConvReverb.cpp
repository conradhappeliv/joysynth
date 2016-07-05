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
    transform(fir.begin(), fir.end(), fir.begin(), bind1st(multiplies<double>(), 1./32768.));
    fir.erase(fir.begin()+1500, fir.end());
}

std::vector<double> ConvReverb::subprocess(const std::vector<double> in){
    return timeConvolve(in, fir);
};