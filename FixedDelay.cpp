#include "FixedDelay.h"

using namespace std;

FixedDelay::FixedDelay() {
    // define the FIR
    FIR.resize(30000);
    for(int i = 0; i < FIR.size(); i++) FIR[i] = 0;
    FIR[0] = 1;
    FIR[10000] = .5;
    FIR[19999] = .25;
    FIR[29999] = .125;
    processed.resize(2000+FIR.size()-1);
}

FixedDelay::~FixedDelay() {}

void FixedDelay::subprocess(const RTArray<double>& input) {
    for(int i = 0; i < FIR.size(); i++) {
        if(FIR[i] != 0) {
            for(int j = 0; j < input.size(); j++)
                processed[i+j] += FIR[i] * input[j];
        }
    }
};