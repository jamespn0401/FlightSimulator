#include "implot.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "project/dashboard/Utils.hpp"

namespace project::dashboard
{

    double padding(double &start, double &end, double paddingFactor)
    {
        return (end - start) * paddingFactor;
    }

    void DrawDashedHorizontalLine(float y, float x_min, float x_max, ImU32 color,
                                  float dashLength, float gapLength)
    {
        ImDrawList *draw_list = ImPlot::GetPlotDrawList();

        ImPlotPoint p1, p2;

        float x = x_min;
        while (x < x_max)
        {
            float x_end = x + dashLength;
            if (x_end > x_max)
                x_end = x_max;

            p1 = ImPlot::PlotToPixels(ImPlotPoint(x, y));
            p2 = ImPlot::PlotToPixels(ImPlotPoint(x_end, y));

            draw_list->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y),
                               color, 2.0f);

            x += dashLength + gapLength;
        }
    }

    void renderThresholdLine(const std::string &title, ControlPanel &controlPanel, float x_min, float x_max, ImU32 color = RED)
    {
        // Get current plot limits
        ImPlotRect limits = ImPlot::GetPlotLimits();
        if (title == "Altitude")
        {
            double target = controlPanel.targetAltitude;
            double upper = target + target * 0.1; // 10% above target
            double lower = target - target * 0.1; // 10% below target
            DrawDashedHorizontalLine(target, limits.X.Min, limits.X.Max,
                                     GREEN);
            DrawDashedHorizontalLine(upper, limits.X.Min, limits.X.Max,
                                     ORANGE);
            DrawDashedHorizontalLine(lower, limits.X.Min, limits.X.Max,
                                     ORANGE);
        }
        else if (title == "Thrust Command")
        {
            double upper = controlPanel.maxThrust;
            double lower = controlPanel.minThrust;
            DrawDashedHorizontalLine(upper, limits.X.Min, limits.X.Max,
                                     ORANGE);
            DrawDashedHorizontalLine(lower, limits.X.Min, limits.X.Max,
                                     ORANGE);
        }
    }

    void render(const std::string &title, std::vector<double> &xs, std::vector<double> &ys, ControlPanel &controlPanel)
    {
        if (ImPlot::BeginPlot(title.c_str()))
        {
            ImPlot::SetupAxes("Time (s)", GRAPHS.at(title).c_str());
            if (!xs.empty())
            {
                double xPadding = padding(xs.front(), xs.back());
                ImPlot::SetupAxisLimits(ImAxis_X1, xs.front() - xPadding, xs.back() + xPadding, ImPlotCond_Always);
                auto [yMinIt, yMaxIt] = std::minmax_element(ys.begin(), ys.end());
                double yPadding = padding(*yMinIt, *yMaxIt);
                ImPlot::SetupAxisLimits(ImAxis_Y1, *yMinIt - yPadding, *yMaxIt + yPadding, ImPlotCond_Always);
                ImPlot::PlotLine(title.c_str(), xs.data(), ys.data(), static_cast<int>(xs.size()));
                renderThresholdLine(title.c_str(), controlPanel, xs.front(), xs.back());
            }
            ImPlot::EndPlot();
        }
    }
}