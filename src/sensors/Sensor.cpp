#include "project/sensors/Sensor.hpp"

namespace project::sensors
{
    Sensor::Sensor(common::Scalar noiseStd, common::Scalar bias)
        : noiseStd_(noiseStd),
          bias_(bias),
          noise_(0.0, noiseStd)
    {
    }

    SensorData Sensor::read(
        const physics::State &state,
        common::Scalar accelerationMetersPerSecondSquared)
    {
        SensorData data{};
        data.altitudeMeters = state.altitudeMeters + bias_ + noise_(rng_);
        data.velocityMetersPerSecond = state.velocityMetersPerSecond;
        data.accelerationMetersPerSecondSquared = accelerationMetersPerSecondSquared;
        return data;
    }

} // namespace project::sensors