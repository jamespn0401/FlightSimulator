#include "project/physics/Dynamics.hpp"

#include "project/common/Constants.hpp"

namespace project::physics
{

    void Dynamics::update(
        State &state,
        common::Scalar thrustAccelerationMetersPerSecondSquared,
        common::Scalar dtSeconds) const
    {
        const common::Scalar netAcceleration =
            thrustAccelerationMetersPerSecondSquared -
            common::kGravityMetersPerSecondSquared;

        state.velocityMetersPerSecond += netAcceleration * dtSeconds;
        state.altitudeMeters += state.velocityMetersPerSecond * dtSeconds;

        if (state.altitudeMeters < 0.0)
        {
            state.altitudeMeters = 0.0;
            state.velocityMetersPerSecond = 0.0;
        }
    }

} // namespace project::physics