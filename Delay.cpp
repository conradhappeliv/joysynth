#include "Delay.h"

using namespace std;

Delay::Delay() {}

Delay::~Delay() {}

vector<double> Delay::subprocess(const vector<double> input) {
    vector<double> res(25000, 0);
    res.insert(res.begin(), input.begin(), input.end());
    transform(res.begin()+25000, res.end(), input.begin(), res.begin()+25000, plus<double>());
    return res;
};