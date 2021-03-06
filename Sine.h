#ifndef JOYSYNTH2_SINE_H
#define JOYSYNTH2_SINE_H

#include <cmath>
#include <vector>
#include "Synthesizer.h"


class Sine : public Synthesizer {
protected:
    virtual void synthesize(std::vector<double>&);
public:
    Sine():Synthesizer() {};
    Sine(int sampRate):Synthesizer(sampRate) {};
};


#endif //JOYSYNTH2_SINE_H
