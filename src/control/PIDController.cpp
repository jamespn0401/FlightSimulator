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
        const common::Scalar error = target - current;

        integral_ += error * dtSeconds;

        common::Scalar derivative = 0.0;
        if (!firstUpdate_ && dtSeconds > 0.0)
        {
            derivative = (error - previousError_) / dtSeconds;
        }

        previousError_ = error;
        firstUpdate_ = false;

        return (kp_ * error) + (ki_ * integral_) + (kd_ * derivative);
    }

    void PIDController::reset()
    {
        integral_ = 0.0;
        previousError_ = 0.0;
        firstUpdate_ = true;
    }

} // namespace project::control