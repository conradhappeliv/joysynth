#ifndef JOYSYNTH2_CONTROLLER_H
#define JOYSYNTH2_CONTROLLER_H

#include <string>
#include <fstream>
#include <thread>
#include <unordered_map>

class Controller {
private:
    struct {
        unsigned int time;    /* event timestamp in milliseconds */
        short value;          /* value */
        unsigned char type;   /* event type */
        unsigned char number; /* axis/button number */
    } js_event;
    std::ifstream file;
    std::unordered_map<int, bool> buttons;
    std::unordered_map<int, short> axes;
    std::thread poll_thread;

    bool init(std::string);
    void get_initial_state();
    void read_message();
    void poll();
public:
    Controller();
    Controller(std::string);
    ~Controller();

    size_t num_of_buttons() { return buttons.size(); }
    size_t num_of_axes() { return axes.size(); }
    bool button(int button_num) { return buttons[button_num]; }
    short axis(int axis_num) { return axes[axis_num]; }
};


#endif //JOYSYNTH2_CONTROLLER_H
