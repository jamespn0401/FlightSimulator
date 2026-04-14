#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "project/control/FlightComputer.hpp"
#include "project/physics/State.hpp"
#include "project/common/Telemetry.hpp"
#include "project/common/TelemetryBuffer.hpp"

namespace project::dashboard
{
    const ImU32 RED = IM_COL32(255, 0, 0, 255);
    const ImU32 GREEN = IM_COL32(0, 255, 0, 255);
    const ImU32 BLUE = IM_COL32(0, 0, 255, 255);
    const ImU32 ORANGE = IM_COL32(255, 165, 0, 255);

    const std::unordered_map<std::string, std::string> GRAPHS = {
        {"Altitude", "Altitude (m)"},
        {"Velocity", "Velocity (m/s)"},
        {"Thrust Command", "Thrust Command (m/s^2)"},
        {"Altitude Error", "Altitude Error (%)"},
    };

    struct ControlPanel
    {
        double kp{0.25};
        double ki{0.01};
        double kd{0.15};
        double minThrust{0.0};
        double maxThrust{20.0};
        double targetAltitude{100.0};
        double dt{0.01};

        bool running{false};
        bool paused{false};
        bool passedTarget{false};

        physics::State state{};
        common::TelemetryBuffer telemetry;
        control::FlightComputer flightComputer;

        ControlPanel() : flightComputer(kp, ki, kd, minThrust, maxThrust), telemetry(6000) {}
    };

    double padding(double &start, double &end, double paddingFactor = 0.1f);
    void DrawDashedHorizontalLine(float y, float x_min, float x_max, ImU32 color = BLUE, float dashLength = 0.2f, float gapLength = 0.1f);
    void render(const std::string &title, std::vector<double> &xs, std::vector<double> &ys, ControlPanel &controlPanel);

}
