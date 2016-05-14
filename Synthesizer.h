#ifndef JOYSYNTH2_SYNTHESIZER_H
#define JOYSYNTH2_SYNTHESIZER_H

#include <vector>
#include <algorithm>

class Synthesizer {
private:
    std::vector<double> remaining;
protected:
    int sampleRate = 44100;
    int bufferSize = 2000;
    double frequency = 440;
    double amplitude = 1;
    virtual void synthesize(std::vector<double>&) = 0;
public:
    Synthesizer() {};
    Synthesizer(int bufSize, int sampRate):
            bufferSize(bufSize),
            sampleRate(sampRate) {};
    void getBuffer(std::vector<double>&);
    void setFrequency(double freq) { frequency = freq; }
    void setAmplitude(double amp) { std::max(0., std::min(1., amp)); }
};

#endif //JOYSYNTH2_SYNTHESIZER_H
