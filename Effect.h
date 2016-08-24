#ifndef JOYSYNTH2_EFFECT_H
#define JOYSYNTH2_EFFECT_H

#include <algorithm>
#include <vector>
#include <iostream>
#include "RTArray.h"

class Effect {
public:
    void process(RTArray<double>&);
    virtual void subprocess(const RTArray<double>&) = 0;
protected:
    int processedsize = 1024;
    std::vector<double> timeConvolve(std::vector<double>, std::vector<double>);
    double* processed;
private:
    std::vector<double> remaining;
};


#endif //JOYSYNTH2_EFFECT_H
