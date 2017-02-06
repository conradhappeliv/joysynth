#ifndef JOYSYNTH2_SAWTOOTH_H
#define JOYSYNTH2_SAWTOOTH_H

#include <cmath>
#include <vector>
#include "Synthesizer.h"

class Sawtooth : public Synthesizer {
private:
    double curx = 0;
protected:
    virtual void synthesize(std::vector<double>&);
public:
    Sawtooth():Synthesizer() {};
    Sawtooth(int sampRate):Synthesizer(sampRate) {};
};


#endif //JOYSYNTH2_TRIANGLE_H
