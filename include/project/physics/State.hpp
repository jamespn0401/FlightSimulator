#pragma once

#include "project/common/Types.hpp"

namespace project::physics
{

    struct State
    {
        common::Scalar altitudeMeters{0.0};
        common::Scalar velocityMetersPerSecond{0.0};
    };

} // namespace project::physics