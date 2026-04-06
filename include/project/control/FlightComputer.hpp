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
        common::Scalar timeSeconds{0.0};
        sensors::SensorData sensorData{};
        common::Scalar thrustCommandMetersPerSecondSquared{0.0};
    };

    class FlightComputer
    {
    public:
        FlightComputer(
            common::Scalar kp,
            common::Scalar ki,
            common::Scalar kd,
            common::Scalar minThrustAcceleration,
            common::Scalar maxThrustAcceleration);

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