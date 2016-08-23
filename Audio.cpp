#include "Audio.h"

Audio::Audio() {
    init_jack();
}

Audio::Audio(bool d, int block_size) {
    debug = d;
    debug_block_size = block_size;
}

void Audio::set_callback(function<vector<double> (int)> f) {
    callback = f;
}

Audio::~Audio() {
    jack_client_close(client);
}

void Audio::init_jack() {
    jack_status_t status;
    client = jack_client_open("JoySynth", JackNullOption, &status, NULL);
    if(client == NULL) {
        std::cerr << "Could not open JACK client" << std::endl;
        return;
    }
    jack_set_process_callback(client, [](jack_nframes_t nframes, void* arg) {
        Audio* a = (Audio*) arg;
        jack_default_audio_sample_t *outbuffer1, *outbuffer2;
        outbuffer1 = (jack_default_audio_sample_t *) jack_port_get_buffer(a->out1, nframes);
        outbuffer2 = (jack_default_audio_sample_t *) jack_port_get_buffer(a->out2, nframes);
        vector<double> res = a->callback(nframes);
        copy(res.begin(), res.end(), outbuffer1);
        copy(res.begin(), res.end(), outbuffer2);
        return 0;
    }, this);

    out1 = jack_port_register(client, "output1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    out2 = jack_port_register(client, "output2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    if(out1 == nullptr || out2 == nullptr) {
        std::cerr << "No more JACK ports available" << std::endl;
        return;
    }
}

void Audio::activate() {
    if(debug) {
        thread([](function<vector<double> (int)> c, int size) {
            while(true) c(size);
        }, callback, debug_block_size);
        return;
    }

    if(jack_activate(client)) {
        std::cerr << "Couldn't activate JACK client" << std::endl;
        return;
    }

    const char** ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
    if (ports == nullptr) {
        std::cerr << "No physical playback ports" << std::endl;
        return;
    }

    if(jack_connect(client,jack_port_name(out1), ports[0]) || jack_connect(client,jack_port_name(out2), ports[1])) {
        std::cerr << "Couldn't connect output ports" << std::endl;
    }
}