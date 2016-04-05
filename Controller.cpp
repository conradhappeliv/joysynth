#include "Controller.h"

using namespace std;

Controller::Controller() {
    init("/dev/input/js0");
}

Controller::Controller(std::string fname) {
    init(fname);
}

bool Controller::init(std::string fname) {
    file.open(fname, ios::in|ios::binary);
    if(!file.is_open()) return false;
    get_initial_state();
    poll_thread = thread(std::bind(&Controller::poll, this));
    return true;
}

void Controller::get_initial_state() {
    read_message();
    while(js_event.type & 0x80) {
        if(js_event.type & 0x1)
            buttons[js_event.number] = js_event.value;
        else // axis
            axes[js_event.number] = js_event.value;
        read_message();
    }
}

void Controller::read_message() {
    file.read((char*) &js_event.time, 4);
    file.read((char*) &js_event.value, 2);
    file.read((char*) &js_event.type, 1);
    file.read((char*) &js_event.number, 1);
}

void Controller::poll() {
    while(true) { // TODO: might be better to make this stop sometime
        read_message();
        if(js_event.type & 0x1)
            buttons[js_event.number] = js_event.value;
        else // axis
            axes[js_event.number] = js_event.value;
    }
}

Controller::~Controller() {
    file.close();
}