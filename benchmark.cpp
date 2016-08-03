#include <vector>

#include "Sine.h"
#include "ConvReverb.h"
#include "Delay.h"
#include <omp.h>

using namespace std;

int main() {
    Sine s(44100);
    ConvReverb rev;

    s.setFrequency(440);
    s.setAmplitude(1);
    auto start = omp_get_wtime();
    for(int i = 0; i < 10000; i++) {
        vector<double> res;
        s.getBuffer(res, 1024);
        rev.process(res);
    }
    auto finish = omp_get_wtime();
    cout << (finish - start) << endl;
}