#include "project/sensors/Sensor.hpp"

namespace project::sensors
{

    SensorData Sensor::read(
        const physics::State &state,
        common::Scalar accelerationMetersPerSecondSquared) const
    {
        SensorData data{};
        data.altitudeMeters = state.altitudeMeters;
        data.velocityMetersPerSecond = state.velocityMetersPerSecond;
        data.accelerationMetersPerSecondSquared = accelerationMetersPerSecondSquared;
        return data;
    }

} // namespace project::sensors