#define TIMEPLOT 0
#define FREQPLOT 1

#include <iostream>
#include <vector>

#include "Controller.h"
#include "Plotter.h"
#include "Synthesizer.h"

using namespace std;

int main() {
    Controller js;
    Plotter p(44100, 32768);
    Synthesizer s(2000, 44100);
    vector<double> in;
    while(true) {
        s.getBuffer(in);
        if(TIMEPLOT || FREQPLOT) p.plot(in, TIMEPLOT, FREQPLOT);
    }
    return 0;
}