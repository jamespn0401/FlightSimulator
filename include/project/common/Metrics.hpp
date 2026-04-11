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
        common::Scalar overshootPercent{0.0}; // How much your system goes past the target altitude before settling.
        common::Scalar settlingTime{0.0};     // How long it takes for altitude to enter and stay within a small error band (usually ±2–5% of target).
        common::Scalar steadyStateError{0.0}; // The final difference between your altitude and the target after everything settles.
        common::Scalar score{0.0};            // Lower score = better controller. Combines overshoot, settling time, and steady-state error into a single metric for easy comparison.
        static Metrics fromSamples(const std::vector<Sample> &samples, common::Scalar target);
    };

    struct Result
    {
        common::Scalar kp{0.0}, ki{0.0}, kd{0.0};
        Metrics metrics;
        static Result fromPID(common::Scalar kp, common::Scalar ki, common::Scalar kd);
    };

}