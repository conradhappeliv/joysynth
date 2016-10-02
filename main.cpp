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
#include "Sawtooth.h"
#include "Triangle.h"
#include "ConvReverb.h"
#include "BiquadLPF.h"

using namespace std;

int main() {
    // program parameters
    int lowest_octave = 1;
    int highest_octave = 8;
    double smoothing = .65; // portamento/rate of change, 0-1 (0 is instant, 1 never moves)

    // globals
    Controller js;
    Plotter p(44100, 32768);
    Sine s1(44100);
    Square s2(44100);
    Sawtooth s3(44100);
    Triangle s4(44100);
    ConvReverb reverb;
    BiquadLPF LPF;
    Audio a;
    int octave = 4;
    double prev_freq = 440;
    bool mod_on = false;
    bool pitch_lock = false;
    bool wave_lock = false;
    bool pitch_snap = false;
    vector<double> chrom_pitches;
    double ampx, ampy;

    // calculate chromatic pitches for snapping
    {
        vector<double> pitch_ratios = {1, 16./15, 1.125, 1.2, 1.25, 4./3, 45. / 32, 1.5, 1.6, 5./3, 1.8, 1.875};
        for (int i = lowest_octave - 1; i < highest_octave + 1; i++) {
            double A = 440 * pow(2, -4 + i);
            for(double ratio: pitch_ratios) {
                chrom_pitches.push_back(A*ratio);
            }
        }
    }

    // assign buttons
    // A B X Y LB RB SEL STA Home LS RS
    js.set_button_press_callback(0, [&mod_on]() { mod_on = true; });
    js.set_button_release_callback(0, [&mod_on]() { mod_on = false; });
    js.set_button_press_callback(1, [&pitch_snap]() { pitch_snap = !pitch_snap; });
    js.set_button_press_callback(2, [&](){ if(octave > lowest_octave) octave--; });
    js.set_button_press_callback(3, [&](){ if(octave < highest_octave) octave++; });
    js.set_button_press_callback(9, [&pitch_lock]() { pitch_lock = !pitch_lock; });
    js.set_button_press_callback(10, [&wave_lock]() { wave_lock = !wave_lock; });

    // main audio callback
    a.set_callback([&](int n) {
        // course pitch
        double newfreq = prev_freq;
        if(!pitch_lock) {
            newfreq = 440*pow(2, -4+octave)*(pow(2, js.axis(1)*-1/32767.));
            // snapping
            if(pitch_snap) {
                auto lower = lower_bound(chrom_pitches.begin(), chrom_pitches.end(), newfreq);
                if(lower != chrom_pitches.end() && lower != chrom_pitches.end()+1) {
                    if(newfreq - *lower > *(lower+1) - newfreq) lower++;
                    newfreq = *lower;
                }
            }
        }
        // fine pitch
        newfreq *= pow(2, js.axis(0)/32767.)/6.-(.5/6+.125-1);
        // portamento
        if(smoothing && !pitch_snap) newfreq = (newfreq*(1-smoothing) + prev_freq*(1+smoothing)) / 2.;
        // send frequency to the synths
        if(!pitch_lock) prev_freq = newfreq;
        s1.setFrequency(newfreq);
        s2.setFrequency(newfreq);
        s3.setFrequency(newfreq);
        s4.setFrequency(newfreq);

        // amplitude
        double overallamp = (js.axis(2) + 32768) / 65536.;
        if(!wave_lock) {
            ampx = js.axis(3) / 32767.;
            ampy = js.axis(4) * -1 / 32767.;
        }
        s1.setAmplitude(overallamp * (.5 + .5 * ampy));
        s2.setAmplitude(overallamp * (.5 + .5 * ampy * -1));
        s3.setAmplitude(overallamp * (.5 + .5 * ampx));
        s4.setAmplitude(overallamp * (.5 + .5 * ampx * -1));

        s1.setMod(mod_on);
        s2.setMod(mod_on);
        s3.setMod(mod_on);
        s4.setMod(mod_on);

        vector<double> res1, res2, res3, res4;
        s1.getBuffer(res1, n);
        s2.getBuffer(res2, n);
        s3.getBuffer(res3, n);
        s4.getBuffer(res4, n);
        transform(res1.begin(), res1.end(), res2.begin(), res1.begin(), plus<double>());
        transform(res1.begin(), res1.end(), res3.begin(), res1.begin(), plus<double>());
        transform(res1.begin(), res1.end(), res4.begin(), res1.begin(), plus<double>());
        transform(res1.begin(), res1.end(), res1.begin(), bind1st(multiplies<double>(), .25));
        //transform(res1.begin(), res1.end(), res1.begin(), bind1st(multiplies<double>(), .15));
        //reverb.process(res1);

        LPF.setCutoff(pow((js.axis(5)+32768.)/65536, 3)*15000+50);
        LPF.process(res1);
        if(TIMEPLOT || FREQPLOT) p.add_data(res1);
        return res1;
    });

    if(TIMEPLOT || FREQPLOT) while(true) p.plot(TIMEPLOT, FREQPLOT);
    else cin.get();
    return 0;
}