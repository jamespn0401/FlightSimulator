#pragma once

#include <vector>
#include <cstddef>
#include "project/common/Telemetry.hpp"

namespace project::common
{

    class TelemetryBuffer // A circular queue buffer to store telemetry data points during the simulation.
    {
    public:
        explicit TelemetryBuffer(std::size_t capacity = 5000);

        void push(const TelemetryPoint &point);       // Adds a new telemetry point to the buffer. If the buffer is full, it overwrites the oldest data.
        std::vector<TelemetryPoint> snapshot() const; // Returns a snapshot of the current telemetry data in the buffer, ordered from oldest to newest.
        void clear();

    private:
        std::vector<TelemetryPoint> data_{};
        std::size_t capacity_{0};
        std::size_t head_{0};
        bool wrapped_{false};
    };
}