#pragma once

#include "project/common/Types.hpp"
#include "project/physics/State.hpp"

namespace project::sensors
{

    struct SensorData
    {
        common::Scalar altitudeMeters{0.0};
        common::Scalar velocityMetersPerSecond{0.0};
        common::Scalar accelerationMetersPerSecondSquared{0.0};
    };

    class Sensor
    {
    public:
        SensorData read(
            const physics::State &state,
            common::Scalar accelerationMetersPerSecondSquared) const;
    };

} // namespace project::sensors