#ifndef JOYSYNTH2_AUDIO_H
#define JOYSYNTH2_AUDIO_H

#include <jack/jack.h>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;

class Audio {
public:
    Audio();
    void set_callback(function<vector<double> (int)>);
    ~Audio();
private:
    std::function<vector<double> (int)> callback = [](int n) { return vector<double>(n); };
    jack_client_t* client;
    jack_port_t *out1, *out2;
    void init_jack();
};


#endif //JOYSYNTH2_AUDIO_H
