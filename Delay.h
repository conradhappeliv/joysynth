#ifndef JOYSYNTH2_DELAY_H
#define JOYSYNTH2_DELAY_H


#include "Effect.h"

class Delay : public Effect {
public:
    Delay();
    ~Delay();
protected:
    virtual void subprocess(const RTArray<double>&);
};


#endif //JOYSYNTH2_DELAY_H
