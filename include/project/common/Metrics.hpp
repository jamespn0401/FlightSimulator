#pragma once

#include <vector>
#include "project/common/Constants.hpp"

namespace project::common
{
    struct Sample
    {
        common::Scalar time;
        common::Scalar altitude;
    };

    struct Metrics
    {
        common::Scalar overshootPercent{0.0};
        common::Scalar settlingTime{0.0};
        common::Scalar steadyStateError{0.0};
        common::Scalar score{0.0};
        static Metrics fromSamples(const std::vector<Sample> &samples, common::Scalar target);
    };

    struct Result
    {
        common::Scalar kp{0.0}, ki{0.0}, kd{0.0};
        Metrics metrics;
        static Result fromPID(common::Scalar kp, common::Scalar ki, common::Scalar kd);
    };

}