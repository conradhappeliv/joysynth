#ifndef JOYSYNTH2_SYNTHESIZER_H
#define JOYSYNTH2_SYNTHESIZER_H

#include <vector>
#include <algorithm>

class Synthesizer {
private:
    int sampleRate = 44100;
    int bufferSize = 2000;
    std::vector<double> remaining;

    void synthesize(std::vector<double>&);
public:
    Synthesizer() {};
    Synthesizer(int bufSize, int sampRate):
            bufferSize(bufSize),
            sampleRate(sampRate) {};
    void getBuffer(std::vector<double>&);
};

#endif //JOYSYNTH2_SYNTHESIZER_H
