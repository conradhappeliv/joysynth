#ifndef JOYSYNTH2_AUDIO_H
#define JOYSYNTH2_AUDIO_H

#include <jack/jack.h>
#include <iostream>
#include <functional>
#include <vector>
#include <thread>

using namespace std;

class Audio {
public:
    Audio();
    Audio(bool, int);
    void set_callback(function<vector<double> (int)>);
    void activate();
    ~Audio();
private:
    std::function<vector<double> (int)> callback = [](int n) { return vector<double>(n); };
    bool debug = false;
    int debug_block_size = 1024;
    jack_client_t* client;
    jack_port_t *out1, *out2;
    void init_jack();
};


#endif //JOYSYNTH2_AUDIO_H
