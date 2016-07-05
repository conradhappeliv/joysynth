#ifndef JOYSYNTH2_CONVREVERB_H
#define JOYSYNTH2_CONVREVERB_H

#include "Effect.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>

class ConvReverb : public Effect {
public:
    ConvReverb();
protected:
    virtual std::vector<double> subprocess(const std::vector<double>);
private:
    std::vector<double> fir;
};


#endif //JOYSYNTH2_CONVREVERB_H
