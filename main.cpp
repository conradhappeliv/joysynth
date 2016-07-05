#define TIMEPLOT 1
#define FREQPLOT 1

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

#include "Controller.h"
#include "Audio.h"
#include "Plotter.h"
#include "Sine.h"
#include "Square.h"
#include "ConvReverb.h"

using namespace std;

int main() {
    // buttons: A B X Y LB RB SEL STA
    Controller js;
    Plotter p(44100, 32768);
    Sine s1(44100);
    Square s2(44100);
    ConvReverb reverb;
    Audio a;

    a.set_callback([&](int n) {
        // course * fine pitch
        double newfreq = 440*(pow(2, js.axis(1)*-1/32767.));
        newfreq *= pow(2, js.axis(0)/32767.)/6.-(.5/6+.125-1);
        s1.setFrequency(newfreq);
        s2.setFrequency(newfreq);

        double overallamp = (js.axis(2) + 32768)/65536.;

        double individualamp = js.axis(4)*-1/32767.;
        s1.setAmplitude(overallamp * (.5 + .5*individualamp));
        s2.setAmplitude(overallamp * (.5 + .5*individualamp *-1));

        vector<double> res1, res2;
        s1.getBuffer(res1, n);
        s2.getBuffer(res2, n);
        transform(res1.begin(), res1.end(), res2.begin(), res1.begin(), plus<double>());
        transform(res1.begin(), res1.end(), res1.begin(), bind1st(multiplies<double>(), .5));
        transform(res1.begin(), res1.end(), res1.begin(), bind1st(multiplies<double>(), .15));
        reverb.process(res1);
        if(TIMEPLOT || FREQPLOT) p.add_data(res1);
        return res1;
    });

    if(TIMEPLOT || FREQPLOT) while(true) p.plot(TIMEPLOT, FREQPLOT);
    else cin.get();
    return 0;
}