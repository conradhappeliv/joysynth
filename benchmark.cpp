#include <vector>
#include <map>

#include "Sine.h"
#include "ConvReverb.h"
#include "Delay.h"
#include <omp.h>

using namespace std;

int main() {
//    Sine s(44100);
//    ConvReverb rev;
//
//    s.setFrequency(440);
//    s.setAmplitude(1);
//    auto start = omp_get_wtime();
//    for(int i = 0; i < 50; i++) {
//        vector<double> res;
//        s.getBuffer(res, 1024);
//        rev.process(res);
//    }
//    auto finish = omp_get_wtime();
//    cout << (finish - start) << endl;
    int lowest_octave = 1;
    int highest_octave = 8;
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

    for(auto scale = scales.begin(); scale != scales.end(); scale++) {
        vector<double> tmp = chrom_pitches;
        vector<double> newtmp;
        int i = 0;
        for (auto pitch = tmp.begin(); pitch != tmp.end(); pitch++) {
            if(find(scale->second.begin(), scale->second.end(), i%12) != scale->second.end()) newtmp.push_back(*pitch);
            i++;
        }
        scale->second = newtmp;
    }

    auto cur_scale = scales.begin();
    cout << "here";
}