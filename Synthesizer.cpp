#include "Synthesizer.h"

using namespace std;

void Synthesizer::synthesize(vector<double>& out) {
    int freq = 880;
    for(int i = 0; i < sampleRate/freq; i++) {
        out.push_back(cos(2.*M_PI*freq*i/sampleRate));
    }
}

void Synthesizer::getBuffer(vector<double>& out) {
    if(!remaining.empty()) {
        if(remaining.size() > bufferSize) {
            move(remaining.begin(), remaining.begin()+bufferSize, out.begin());
            return;
        } else {
            swap(remaining, out);
            remaining.clear();
        }
    }
    while(out.size() < bufferSize) synthesize(out);
    if(out.size() > bufferSize) {
        remaining.assign(out.begin()+bufferSize, out.end());
        out.erase(out.begin()+bufferSize, out.end());
    }
}

