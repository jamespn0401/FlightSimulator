#pragma once

#include "project/common/Types.hpp"

namespace project::control
{

    class PIDController
    {
    public:
        PIDController(
            common::Scalar kp,
            common::Scalar ki,
            common::Scalar kd);

        common::Scalar compute(
            common::Scalar target,
            common::Scalar current,
            common::Scalar dtSeconds);

        void reset();

    private:
        common::Scalar kp_{0.0};
        common::Scalar ki_{0.0};
        common::Scalar kd_{0.0};

        common::Scalar integral_{0.0};
        common::Scalar previousError_{0.0};
        bool firstUpdate_{true};
    };

} // namespace project::control