#include <cassert>
#include <cmath>
#include <iostream>

#include "project/physics/Dynamics.hpp"
#include "project/physics/State.hpp"

int main()
{
    project::physics::Dynamics dynamics;
    project::physics::State state{};

    state.altitudeMeters = 0.0;
    state.velocityMetersPerSecond = 0.0;

    dynamics.update(state, 20.0, 0.1);

    assert(state.altitudeMeters >= 0.0);
    std::cout << "Passed altitude test: " << state.altitudeMeters << " meters" << std::endl;
    assert(state.velocityMetersPerSecond > 0.0);
    std::cout << "Passed velocity test: " << state.velocityMetersPerSecond << " m/s" << std::endl;

    project::physics::State groundState{};
    groundState.altitudeMeters = 0.0;
    groundState.velocityMetersPerSecond = -5.0;

    dynamics.update(groundState, 0.0, 0.1);

    assert(std::abs(groundState.altitudeMeters - 0.0) < 1e-9);
    std::cout << "Passed ground altitude test: " << groundState.altitudeMeters << " meters" << std::endl;
    assert(std::abs(groundState.velocityMetersPerSecond - 0.0) < 1e-9);
    std::cout << "Passed ground velocity test: " << groundState.velocityMetersPerSecond << " m/s" << std::endl;

    std::cout << "All Dynamics tests passed!" << std::endl;
    return 0;
}