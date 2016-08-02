#include "Synthesizer.h"

using namespace std;

void Synthesizer::init() {
    double mod_depth = .1;
    for(int i = 0; i < 44100/7; i++) {
        mod_lfo[i] = pow(2, mod_depth * sin(i * 2. * M_PI * 7 / 44100));
    }
    mod_pos = mod_lfo.begin();
}

void Synthesizer::getBuffer(vector<double>& out, int size = 0) {
    out.reserve(size);
    if(!remaining.empty()) {
        if(remaining.size() > size) {
            out.resize(size);
            move(remaining.begin(), remaining.begin()+size, out.begin());
            remaining.erase(remaining.begin(), remaining.begin()+size);
        } else {
            swap(remaining, out);
            remaining.clear();
        }
    }
    double tmpfreq = frequency;
    while(out.size() < size) {
        if(mod_on) {
            frequency = tmpfreq*(*mod_pos);
            mod_pos += (int)(sampleRate/frequency);
            if(mod_pos >= mod_lfo.end()) mod_pos -= (int) mod_lfo.size();
        }
        synthesize(out);
    }
    frequency = tmpfreq;
    if(out.size() > size) {
        remaining.assign(out.begin()+size, out.end());
        out.erase(out.begin()+size, out.end());
    }
    transform(out.begin(), out.end(), out.begin(), bind1st(multiplies<double>(), amplitude));
}

