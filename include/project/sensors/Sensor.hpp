#pragma once

#include <random>
#include <algorithm>
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
        Sensor(common::Scalar noiseStd, common::Scalar bias);
        SensorData read(
            const physics::State &state,
            common::Scalar accelerationMetersPerSecondSquared);

    private:
        common::Scalar noiseStd_{0.0};
        common::Scalar bias_{0.0};
        std::mt19937 rng_{std::random_device{}()};
        std::normal_distribution<common::Scalar> noise_;
    };

} // namespace project::sensors