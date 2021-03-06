#ifndef JOYSYNTH2_TRIANGLE_H
#define JOYSYNTH2_TRIANGLE_H

#include <cmath>
#include <vector>
#include "Synthesizer.h"

class Triangle : public Synthesizer {
private:
    double curx = 0;
    int pos = 1;
protected:
    virtual void synthesize(std::vector<double>&);
public:
    Triangle():Synthesizer() {};
    Triangle(int sampRate):Synthesizer(sampRate) {};
};


#endif //JOYSYNTH2_TRIANGLE_H
