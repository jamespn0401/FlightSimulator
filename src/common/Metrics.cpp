#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <fstream>
#include <iomanip>

#include "project/common/Metrics.hpp"
#include "project/common/Constants.hpp"
#include "project/control/FlightComputer.hpp"
#include "project/physics/State.hpp"

namespace project::common
{
    Metrics Metrics::fromSamples(
        const std::vector<Sample> &samples,
        common::Scalar target)
    {
        Metrics m{};

        if (samples.empty() || target <= 0.0)
        {
            m.score = std::numeric_limits<common::Scalar>::infinity();
            return m;
        }

        common::Scalar maxAltitude = samples.front().altitude;

        for (const auto &s : samples)
        {
            if (s.altitude > maxAltitude)
                maxAltitude = s.altitude;
        }

        common::Scalar overshoot = std::max(0.0, maxAltitude - target);
        m.overshootPercent = (overshoot / target) * 100.0;

        // settling time (2% band)
        common::Scalar band = target * 0.02;
        m.settlingTime = samples.back().time;

        for (size_t i = 0; i < samples.size(); ++i)
        {
            bool settled = true;

            for (size_t j = i; j < samples.size(); ++j)
            {
                if (std::abs(samples[j].altitude - target) > band)
                {
                    settled = false;
                    break;
                }
            }

            if (settled)
            {
                m.settlingTime = samples[i].time;
                break;
            }
        }

        m.steadyStateError =
            std::abs(samples.back().altitude - target);

        // score
        m.score =
            m.overshootPercent * 3.0 +
            m.settlingTime * 1.0 +
            m.steadyStateError * 10.0;

        return m;
    }

    Result Result::fromPID(common::Scalar kp, common::Scalar ki, common::Scalar kd)
    {
        constexpr common::Scalar dt = 0.01;
        constexpr common::Scalar duration = 30.0;
        constexpr common::Scalar target = 100.0;

        project::physics::State state{};

        project::control::FlightComputer fc(
            kp, ki, kd,
            0.0, 20.0);

        std::vector<Sample> samples;

        for (common::Scalar t = 0.0; t <= duration; t += dt)
        {
            auto result = fc.step(state, target, dt, t);

            samples.push_back({result.timeSeconds,
                               result.sensorData.altitudeMeters});
        }

        Result r{};
        r.kp = kp;
        r.ki = ki;
        r.kd = kd;
        r.metrics = Metrics::fromSamples(samples, target);
        return r;
    }
}