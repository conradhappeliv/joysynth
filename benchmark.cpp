#include <vector>

#include "Sine.h"
#include "ConvReverb.h"

using namespace std;

int main() {
    Sine s(44100);
    ConvReverb rev;

    s.setFrequency(440);
    s.setAmplitude(1);

    while(true) {
        vector<double> res;
        s.getBuffer(res, 1024);
        rev.process(res);
    }
}