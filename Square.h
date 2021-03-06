#ifndef JOYSYNTH2_SQUARE_H
#define JOYSYNTH2_SQUARE_H

#include <cmath>
#include <vector>
#include "Synthesizer.h"

class Square : public Synthesizer {
protected:
    virtual void synthesize(std::vector<double>&);
public:
    Square():Synthesizer() {};
    Square(int sampRate):Synthesizer(sampRate) {};
};


#endif //JOYSYNTH2_SQUARE_H
