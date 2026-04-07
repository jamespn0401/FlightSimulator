#include "project/control/FlightComputer.hpp"
#include "project/common/Constants.hpp"

namespace project::control
{

    FlightComputer::FlightComputer(
        common::Scalar kp,
        common::Scalar ki,
        common::Scalar kd,
        common::Scalar minThrustAcceleration,
        common::Scalar maxThrustAcceleration)
        : pid_(kp, ki, kd),
          sensor_(0.0, 0.0),
          minThrustAcceleration_(minThrustAcceleration),
          maxThrustAcceleration_(maxThrustAcceleration)
    {
    }
    //
    StepResult FlightComputer::step(
        physics::State &state,
        common::Scalar targetAltitudeMeters,
        common::Scalar dtSeconds,
        common::Scalar timeSeconds)
    {
        const sensors::SensorData currentSensorData =
            sensor_.read(state, 0.0);

        common::Scalar thrustCommand =
            pid_.compute(
                targetAltitudeMeters,
                currentSensorData.altitudeMeters,
                dtSeconds);

        thrustCommand = clamp(
            thrustCommand,
            minThrustAcceleration_,
            maxThrustAcceleration_);

        dynamics_.update(state, thrustCommand, dtSeconds);

        const common::Scalar actualAcceleration =
            thrustCommand - common::kGravityMetersPerSecondSquared;

        return StepResult{
            timeSeconds,
            sensor_.read(state, actualAcceleration),
            thrustCommand};
    }

    common::Scalar FlightComputer::clamp(
        common::Scalar value,
        common::Scalar minValue,
        common::Scalar maxValue)
    {
        if (value < minValue)
        {
            return minValue;
        }
        if (value > maxValue)
        {
            return maxValue;
        }
        return value;
    }

} // namespace project::control