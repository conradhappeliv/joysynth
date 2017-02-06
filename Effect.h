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
    void timeConvolve(const RTArray<double>& a, const RTArray<double>& b, RTArray<double>& res);
    RTArray<double> processed;
private:
    std::vector<double> remaining;
};


#endif //JOYSYNTH2_EFFECT_H
