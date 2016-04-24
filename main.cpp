#include <iostream>
#include <cmath>
#include <vector>

#include "Controller.h"
#include "Plotter.h"

using namespace std;

int main() {
    Controller js;
    Plotter p(44100, 32768);
    int i = 0;
    while(true) {
        vector<double> in;
        for(int j = 0; j < 2000; j++)
            in.push_back(cos((double)i++/44100.*440.*2.*M_PI));
        if(i > 44100) i -= 44100;
        p.plot(in);
    }
    return 0;
}