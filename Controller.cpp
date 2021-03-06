#include "Controller.h"

using namespace std;

Controller::Controller() : Controller("/dev/input/js0") { }

Controller::Controller(std::string fname) {
    if(!init(fname)) {
        cerr << "UNABLE TO OPEN " << fname << endl;
    };
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
        if(js_event.type & 0x1) {
            if(buttons[js_event.number] != (bool) js_event.value) {
                if(js_event.value == 1 && on_button_press.count(js_event.number)) on_button_press[js_event.number]();
                else if(js_event.value == 0 && on_button_release.count(js_event.number)) on_button_release[js_event.number]();
            }
            buttons[js_event.number] = js_event.value;
        } else // axis
            if(axes[js_event.number] < 30000 && js_event.value >= 30000 && on_axis_up.count(js_event.number)) on_axis_up[js_event.number]();
            else if(axes[js_event.number] > -30000 && js_event.value <= -30000 && on_axis_down.count(js_event.number)) on_axis_down[js_event.number]();
            axes[js_event.number] = js_event.value;
    }
}

Controller::~Controller() {
    file.close();
}