#pragma once

#include "project/common/Types.hpp"
#include "project/physics/State.hpp"

namespace project::physics
{

    class Dynamics
    {
    public:
        void update(
            State &state,
            common::Scalar thrustAccelerationMetersPerSecondSquared,
            common::Scalar dtSeconds) const;
    };

} // namespace project::physics