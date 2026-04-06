#include <iomanip>
#include <iostream>

#include "project/control/FlightComputer.hpp"
#include "project/physics/State.hpp"

int main()
{
    constexpr project::common::Scalar kDtSeconds = 0.1;
    constexpr project::common::Scalar kSimulationDurationSeconds = 20.0;
    constexpr project::common::Scalar kTargetAltitudeMeters = 100.0;

    project::physics::State state{};
    state.altitudeMeters = 0.0;
    state.velocityMetersPerSecond = 0.0;

    project::control::FlightComputer flightComputer(
        0.25, // kp
        0.01, // ki
        0.15, // kd
        0.0,  // min thrust acceleration
        20.0  // max thrust acceleration
    );

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Time\tAlt\tVel\tAccel\tThrustCmd\n";

    for (project::common::Scalar timeSeconds = 0.0;
         timeSeconds <= kSimulationDurationSeconds;
         timeSeconds += kDtSeconds)
    {
        const project::control::StepResult result =
            flightComputer.step(
                state,
                kTargetAltitudeMeters,
                kDtSeconds,
                timeSeconds);

        std::cout
            << result.timeSeconds << '\t'
            << result.sensorData.altitudeMeters << '\t'
            << result.sensorData.velocityMetersPerSecond << '\t'
            << result.sensorData.accelerationMetersPerSecondSquared << '\t'
            << result.thrustCommandMetersPerSecondSquared << '\n';
    }

    return 0;
}