#include "Synthesizer.h"

using namespace std;

void Synthesizer::init() {
    double mod_depth = .1;
    for(int i = 0; i < 44100/7; i++) {
        mod_lfo[i] = pow(2, mod_depth * sin(i * 2. * M_PI * 7 / 44100));
    }
    mod_pos = mod_lfo.begin();
}

void Synthesizer::getBuffer(RTArray<double> out) {
    size_t size = out.size();
    size_t insertedsize = 0;
    if(!remaining.empty()) {
        if(remaining.size() > size) {
            for(size_t i = 0; i < size; i++) out[i] = remaining[i];
            insertedsize = size;
            remaining.erase(remaining.begin(), remaining.begin()+size);
        } else {
            for(size_t i = 0; i < remaining.size(); i++) out[i] = remaining[i];
            insertedsize += remaining.size();
            remaining.clear();
        }
    }
    double tmpfreq = frequency;
    while(insertedsize < size) {
        if(mod_on) {
            frequency = tmpfreq*(*mod_pos);
            mod_pos += (int)(sampleRate/frequency);
            if(mod_pos >= mod_lfo.end()) mod_pos -= (int) mod_lfo.size();
        }
        processed.clear();
        synthesize(processed);
        if(insertedsize + processed.size() > size) {
            for(size_t i = 0; i < size-insertedsize; i++) out[insertedsize+i] = processed[i];
            for(size_t i = size-insertedsize; i < processed.size(); i++) remaining.push_back(processed[i]);
            insertedsize = size;
        } else {
            for(size_t i = 0; i < processed.size(); i++) out[insertedsize+i] = processed[i];
            insertedsize += processed.size();
        }
    }

    for(int i = 0; i < size; i++) out[i] *= amplitude;
}

