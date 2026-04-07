
#include <algorithm>
#include "project/control/PIDController.hpp"

namespace project::control
{

    PIDController::PIDController(
        common::Scalar kp,
        common::Scalar ki,
        common::Scalar kd)
        : kp_(kp),
          ki_(ki),
          kd_(kd)
    {
    }

    common::Scalar PIDController::compute(
        common::Scalar target,
        common::Scalar current,
        common::Scalar dtSeconds)
    {
        if (dtSeconds <= 0.0)
        {
            return 0.0;
        }
        const common::Scalar minOutput = 0.0;
        const common::Scalar maxOutput = 20.0;
        const common::Scalar error = target - current;

        integral_ += error * dtSeconds;

        common::Scalar derivative = (!firstUpdate_ && dtSeconds > 0.0) ? (error - previousError_) / dtSeconds : 0.0;

        const common::Scalar provisionalIntegral = integral_ + error * dtSeconds;
        const common::Scalar unsaturatedOutput =
            (kp_ * error) +
            (ki_ * provisionalIntegral) +
            (kd_ * derivative);
        const common::Scalar saturatedOutput =
            std::clamp(unsaturatedOutput, minOutput, maxOutput);
        const bool isSaturatedHigh = (saturatedOutput >= maxOutput);
        const bool isSaturatedLow = (saturatedOutput <= minOutput);
        const bool drivingMorePositive = (error > 0.0);
        const bool drivingMoreNegative = (error < 0.0);
        const bool allowIntegralUpdate =
            (!isSaturatedHigh && !isSaturatedLow) ||
            (isSaturatedHigh && drivingMoreNegative) ||
            (isSaturatedLow && drivingMorePositive);

        if (allowIntegralUpdate)
        {
            integral_ = provisionalIntegral;
        }

        const common::Scalar output =
            (kp_ * error) +
            (ki_ * integral_) +
            (kd_ * derivative);

        previousError_ = error;
        firstUpdate_ = false;

        return std::clamp(output, minOutput, maxOutput);
    }

    void PIDController::reset()
    {
        integral_ = 0.0;
        previousError_ = 0.0;
        firstUpdate_ = true;
    }

} // namespace project::control