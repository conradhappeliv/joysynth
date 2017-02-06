#ifndef JOYSYNTH2_DELAY_H
#define JOYSYNTH2_DELAY_H

#include <vector>

#include "Effect.h"

class FixedDelay : public Effect {
private:
    RTArray<double> FIR;
public:
    FixedDelay();
    ~FixedDelay();
protected:
    virtual void subprocess(const RTArray<double>&);
};


#endif //JOYSYNTH2_DELAY_H
