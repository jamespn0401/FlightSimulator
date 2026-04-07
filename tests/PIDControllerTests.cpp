#include <cassert>
#include <cmath>
#include <iostream>

#include "project/control/PIDController.hpp"

int main()
{
    project::control::PIDController pid(1.0, 0.0, 0.0);

    const double output1 = pid.compute(10.0, 8.0, 0.1);
    assert(std::abs(output1 - 2.0) < 1e-9);
    std::cout << "Passed first compute test: " << output1 << std::endl;

    pid.reset();

    const double output2 = pid.compute(5.0, 1.0, 0.1);
    assert(std::abs(output2 - 4.0) < 1e-9);
    std::cout << "Passed second compute test: " << output2 << std::endl;

    std::cout << "All PIDController tests passed!" << std::endl;
    return 0;
}