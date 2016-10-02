#ifndef JOYSYNTH2_SYNTHESIZER_H
#define JOYSYNTH2_SYNTHESIZER_H

#include <vector>
#include <array>
#include <algorithm>
#include "RTArray.h"

class Synthesizer {
private:
    void init();
    bool mod_on = false;
    std::array<double, 44100/7>::iterator mod_pos;
    std::vector<double> remaining;
    std::array<double, 44100/7> mod_lfo;
protected:
    int sampleRate = 44100;
    double frequency = 440;
    double amplitude = 1;
    virtual void synthesize(std::vector<double>&) = 0;
    std::vector<double> processed;
    double phase = 0;
public:
    Synthesizer() { init(); };
    Synthesizer(int sampRate):
            sampleRate(sampRate) { init(); };
    void getBuffer(RTArray<double>&);
    void setFrequency(double freq) { frequency = freq; }
    void setAmplitude(double amp) { amplitude = std::max(0., std::min(1., amp)); }
    void setMod(bool on) { mod_on = on; };
};

#endif //JOYSYNTH2_SYNTHESIZER_H
