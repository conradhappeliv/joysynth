#include "Effect.h"

using namespace std;

void Effect::process(vector<double>& buffer) {
    // do the effect processing
    processed.clear();
    subprocess(buffer);
    // zero buffer (may not be necessary)
    for(auto i = buffer.begin(); i != buffer.end(); i++) *i = 0;
    // move any feedback into the result
    int amt = min(remaining.size(), buffer.size());
    move(remaining.begin(), remaining.begin() + amt, buffer.begin());
    remaining.erase(remaining.begin(), remaining.begin() + amt);
    // add the newly-processed samples
    for(int i = 0; i < buffer.size(); i++) buffer[i] = buffer[i] + processed[i];
    // erase what we just added and add the remainder to the feedback vector
    processed.erase(processed.begin(), processed.begin() + buffer.size());
    if(remaining.size() < processed.size())
        remaining.resize(processed.size());
    for(int i = 0; i < processed.size(); i++) remaining[i] = remaining[i] + processed[i];
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