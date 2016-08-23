#ifndef JOYSYNTH2_EFFECT_H
#define JOYSYNTH2_EFFECT_H

#include <algorithm>
#include <vector>
#include <iostream>

class Effect {
public:
    void process(std::vector<double>&);
    virtual void subprocess(const std::vector<double>) = 0;
protected:
    std::vector<double> timeConvolve(std::vector<double>, std::vector<double>);
    std::vector<double> processed;
private:
    std::vector<double> remaining;
};


#endif //JOYSYNTH2_EFFECT_H
