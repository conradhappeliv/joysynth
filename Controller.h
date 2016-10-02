#ifndef JOYSYNTH2_CONTROLLER_H
#define JOYSYNTH2_CONTROLLER_H

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
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
    std::unordered_map<int, std::function<void()>> on_button_press;
    std::unordered_map<int, std::function<void()>> on_button_release;
    std::unordered_map<int, std::function<void()>> on_axis_up;
    std::unordered_map<int, std::function<void()>> on_axis_down;

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
    void set_button_press_callback(int button_num, std::function<void()> fun) { on_button_press[button_num] = fun; }
    void set_button_release_callback(int button_num, std::function<void()> fun) { on_button_release[button_num] = fun; }
    void set_axis_as_button_up(int axis_num, std::function<void()> fun) { on_axis_up[axis_num] = fun; }
    void set_axis_as_button_down(int axis_num, std::function<void()> fun) { on_axis_down[axis_num] = fun; }
};


#endif //JOYSYNTH2_CONTROLLER_H
