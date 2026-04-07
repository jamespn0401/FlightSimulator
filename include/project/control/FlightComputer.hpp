#pragma once

#include "project/common/Types.hpp"
#include "project/control/PIDController.hpp"
#include "project/physics/Dynamics.hpp"
#include "project/physics/State.hpp"
#include "project/sensors/Sensor.hpp"

namespace project::control
{

    struct StepResult
    {
        common::Scalar timeSeconds{0.0}; // Current simulation time (seconds)
        sensors::SensorData sensorData{};
        common::Scalar thrustCommandMetersPerSecondSquared{0.0}; // Output command: desired thrust acceleration (m/s^2) sent to the system
    };

    class FlightComputer
    {
    public:
        FlightComputer(
            common::Scalar kp,                    // Proportional gain for PID (reacts to current error)
            common::Scalar ki,                    // Integral gain (accumulates past error)
            common::Scalar kd,                    // Derivative gain (reacts to rate of error change)
            common::Scalar minThrustAcceleration, // Minimum allowed thrust acceleration (lower bound)
            common::Scalar maxThrustAcceleration  // Maximum allowed thrust acceleration (upper bound)
        );

        StepResult step(
            physics::State &state,
            common::Scalar targetAltitudeMeters,
            common::Scalar dtSeconds,
            common::Scalar timeSeconds);

    private:
        static common::Scalar clamp(
            common::Scalar value,
            common::Scalar minValue,
            common::Scalar maxValue);

        PIDController pid_;
        physics::Dynamics dynamics_;
        sensors::Sensor sensor_;

        common::Scalar minThrustAcceleration_{0.0};
        common::Scalar maxThrustAcceleration_{0.0};
    };

} // namespace project::control