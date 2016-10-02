#define TIMEPLOT 0
#define FREQPLOT 0

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <algorithm>
#include <functional>
#include <boost/filesystem.hpp>

#include "RTArray.h"
#include "Controller.h"
#include "Audio.h"
#include "Plotter.h"
#include "Sine.h"
#include "Square.h"
#include "Sawtooth.h"
#include "Triangle.h"
#include "ConvReverb.h"
#include "Delay.h"

using namespace std;

int main() {
    // if fftw wisdom has been saved here before, load it
    if(boost::filesystem::exists("./fftw_wisdom.txt"))
        fftw_import_wisdom_from_filename("./fftw_wisdom.txt");

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
    Audio a;
    Delay d;
    int octave = 4;
    double prev_freq = 440;
    bool activated = true;
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

    map<string, vector<double>> scales;
    scales["major"] = {0, 2, 4, 5, 7, 9, 11};
    scales["minor"] = {0, 2, 3, 5, 7, 8, 10};
    scales["jazz"] = {0, 3, 5, 6, 7, 10, 11};

    for(auto scale = scales.begin(); scale != scales.end(); scale++) {
        vector<double> newtmp;
        int i = 0;
        for (auto pitch = chrom_pitches.begin(); pitch != chrom_pitches.end(); pitch++) {
            if(find(scale->second.begin(), scale->second.end(), i%12) != scale->second.end()) newtmp.push_back(*pitch);
            i++;
        }
        scale->second = newtmp;
    }

    auto cur_scale = scales.begin();

    // assign buttons
    // A B X Y LB RB SEL STA Home LS RS
    js.set_button_press_callback(0, [&mod_on]() { mod_on = true; });
    js.set_button_release_callback(0, [&mod_on]() { mod_on = false; });
    js.set_button_press_callback(1, [&pitch_snap]() { pitch_snap = !pitch_snap; });
    js.set_button_press_callback(2, [&](){ if(octave > lowest_octave) octave--; });
    js.set_button_press_callback(3, [&](){ if(octave < highest_octave) octave++; });
    js.set_button_press_callback(9, [&pitch_lock]() { pitch_lock = !pitch_lock; });
    js.set_button_press_callback(10, [&wave_lock]() { wave_lock = !wave_lock; });
    js.set_axis_as_button_up(7, [&cur_scale, &scales, &cout]() { if(cur_scale != scales.begin()) --cur_scale; cout << (*cur_scale).first << endl; });
    js.set_axis_as_button_down(7, [&cur_scale, &scales, &cout]() { if(cur_scale != (--scales.rbegin()).base()) ++cur_scale; cout << (*cur_scale).first << endl; });

    RTArray<double> res1(1024), res2(1024), res3(1024), res4(1024);

    // main audio callback
    a.set_callback([&](int n) {
        if(activated) {
            // course pitch
            double newfreq = prev_freq;
            if (!pitch_lock) {
                newfreq = 440 * pow(2, -4 + octave) * (pow(2, js.axis(1) * -1 / 32767.));
                // snapping
                if (pitch_snap) {
                    auto source = cur_scale->second;
                    auto lower = lower_bound(source.begin(), source.end(), newfreq);
                    if (lower != source.end() && lower != source.end() + 1) {
                        if (newfreq - *lower > *(lower + 1) - newfreq) lower++;
                        newfreq = *lower;
                    }
                }
            }
            // fine pitch
            newfreq *= pow(2, js.axis(0) / 32767.) / 6. - (.5 / 6 + .125 - 1);
            // portamento
            if (smoothing && !pitch_snap) newfreq = (newfreq * (1 - smoothing) + prev_freq * (1 + smoothing)) / 2.;
            // send frequency to the synths
            if (!pitch_lock) prev_freq = newfreq;
            s1.setFrequency(newfreq);
            s2.setFrequency(newfreq);
            s3.setFrequency(newfreq);
            s4.setFrequency(newfreq);

            // amplitude
            double overallamp = (js.axis(2) + 32768) / 65536. * .4;
            if (!wave_lock) {
                ampx = js.axis(3) / 32767.;
                ampy = js.axis(4) * -1 / 32767.;
            }
            s1.setAmplitude(overallamp * (.5 + .5 * ampy));
            s2.setAmplitude(overallamp * (.5 + .5 * ampy * -1));
            s3.setAmplitude(0);//overallamp * (.5 + .5 * ampx));
            s4.setAmplitude(0);//overallamp * (.5 + .5 * ampx * -1));

            s1.setMod(mod_on);
            s2.setMod(mod_on);
            s3.setMod(mod_on);
            s4.setMod(mod_on);

            s1.getBuffer(res1);
            s2.getBuffer(res2);
            s3.getBuffer(res3);
            s4.getBuffer(res4);
            for(int i = 0; i < n; i++) {
                res1[i] += res2[i] + res3[i] + res4[i];
                res1[i] *= .25;
            }
        } else {
            res1.zero();
        }

        //reverb.setAmount((js.axis(5) + 32768) / 65536. /2);
        //reverb.process(res1);
        //d.process(res1);
        if(TIMEPLOT || FREQPLOT) p.add_data(res1);
        return &res1;
    });

    // begin the program
    a.activate();

    // save FFTW wisdom to file for later usage
    fftw_export_wisdom_to_filename("./fftw_wisdom.txt");

    if(TIMEPLOT || FREQPLOT) while(true) p.plot(TIMEPLOT, FREQPLOT);
    else {
        while(true) {
            string line;
            cin >> line;
            if(line == "up") octave++;
            else if(line == "down") octave--;
            else if(line == "stop") activated = false;
            else if(line == "start") activated = true;
            else if(line == "mod") mod_on = !mod_on;
            else if(line == "snap") pitch_snap = !pitch_snap;
            else if(line == "exit") return 0;
        }
    }
}