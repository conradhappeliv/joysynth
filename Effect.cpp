#include "Effect.h"

using namespace std;

void Effect::process(RTArray<double>& buffer) {
    size_t size = buffer.size();
    for(int i = 0; i < processed.size(); i++) processed[i] = 0;
    // do the effect processing
    subprocess(buffer);
    // zero buffer (may not be necessary)
    for(auto i = 0; i != size; i++) buffer[i] = 0;
    // move any feedback into the result
    int amt = min(remaining.size(), size);
    for(size_t i = 0; i < amt; i++) buffer[i] = remaining[i];
    remaining.erase(remaining.begin(), remaining.begin() + amt);
    // add the newly-processed samples
    for(int i = 0; i < size; i++) buffer[i] = buffer[i] + processed[i];
    // erase what we just added and add the remainder to the feedback vector
    if(processed.size()> size) {
        if(remaining.size() < processed.size()-size) {
            remaining.insert(remaining.end(), processed.size()-size-remaining.size(), 0);
        }
        for(int i = size; i < processed.size(); i++) remaining[i-size] += processed[i];
    }
}

// based on: http://stackoverflow.com/a/8425094
vector<double> Effect::timeConvolve(vector<double> a, vector<double> b) {
    vector<double> result;
    reverse(b.begin(), b.end());
    result.insert(result.begin(), a.size() + b.size() - 1, 0);
    for(int i = 0; i <  a.size() + b.size() - 1; i++) {
        int min = (i >= b.size()-1) ? i - (b.size()-1) : 0;
        int max = (i < a.size()-1) ? i : a.size()-1;
        for(int j = min; j <= max; j++) {
            result[i] += a[j] * b[i-j];
        }
    }
    return result;
}