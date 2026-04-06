#include <iostream>
#include "Utils/utils.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    bool running{true};
    size_t iteration{0};
    while (running)
    {
        // sensor_data = read_sensors();
        // state = estimate_state(sensor_data);
        // control_output = controller(state);
        // apply_actuators(control_output);
        // log_telemetry(state, control_output);
        if (++iteration > 10)
        {
            running = false;
        }
    }

    cout << "End!" << endl;
    return 0;
}
