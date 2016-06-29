#define TIMEPLOT 1
#define FREQPLOT 1

#include <iostream>
#include <vector>

#include "Controller.h"
#include "Audio.h"
#include "Plotter.h"
#include "Sine.h"
#include "Square.h"

using namespace std;

int main() {
    Controller js;
    Plotter p(44100, 32768);
    Square s(44100);
    Audio a;

    a.set_callback([&](int n) {
        vector<double> res;
        s.getBuffer(res, n);
        if(TIMEPLOT || FREQPLOT) p.add_data(res);
        return res;
    });
    if(TIMEPLOT || FREQPLOT) while(true) p.plot(TIMEPLOT, FREQPLOT);
    else cin.get();
    return 0;
}