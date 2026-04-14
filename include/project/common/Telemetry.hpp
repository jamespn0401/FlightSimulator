#pragma once

#include "project/common/Types.hpp"

namespace project::common
{
    struct TelemetryPoint
    {
        Scalar timeSeconds{0.0};
        Scalar altitudeMeters{0.0};
        Scalar velocityMetersPerSecond{0.0};
        Scalar accelerationMetersPerSecondSquared{0.0};
        Scalar thrustCommandMetersPerSecondSquared{0.0};
        Scalar targetAltitudeMeters{0.0};
        Scalar altitudeErrorMeters{0.0};
        Scalar altitudeErrorPercentage{0.0};
    };

}