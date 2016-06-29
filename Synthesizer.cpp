#include "Synthesizer.h"

using namespace std;

void Synthesizer::getBuffer(vector<double>& out, int size = 0) {
    if(!remaining.empty()) {
        if(remaining.size() > size) {
            move(remaining.begin(), remaining.begin()+size, out.begin());
            return;
        } else {
            swap(remaining, out);
            remaining.clear();
        }
    }
    while(out.size() < size) synthesize(out);
    if(out.size() > size) {
        remaining.assign(out.begin()+size, out.end());
        out.erase(out.begin()+size, out.end());
    }
}

