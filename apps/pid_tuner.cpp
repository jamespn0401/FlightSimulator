#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <fstream>
#include <iomanip>

#include "project/control/FlightComputer.hpp"
#include "project/physics/State.hpp"
#include "project/common/Metrics.hpp"

int main()
{
    std::vector<project::common::Result> allResults;

    project::common::Result best;
    best.metrics.score = std::numeric_limits<double>::infinity();

    // coarse grid search
    for (project::common::Scalar kp = 0.1; kp <= 1.0; kp += 0.1)
    {
        for (project::common::Scalar ki = 0.0; ki <= 0.1; ki += 0.02)
        {
            for (project::common::Scalar kd = 0.0; kd <= 0.5; kd += 0.05)
            {
                project::common::Result r = project::common::Result::fromPID(kp, ki, kd);
                allResults.push_back(r);

                std::cout << std::fixed << std::setprecision(3)
                          << "kp=" << kp
                          << ", ki=" << ki
                          << ", kd=" << kd
                          << " | score=" << r.metrics.score
                          << "\n";

                if (r.metrics.score < best.metrics.score)
                {
                    best = r;
                }
            }
        }
    }

    // -----------------------------
    // Output best result
    // -----------------------------
    std::cout << "\n===== BEST PID =====\n";
    std::cout << "kp=" << best.kp
              << ", ki=" << best.ki
              << ", kd=" << best.kd << "\n";

    std::cout << "Overshoot: " << best.metrics.overshootPercent << "%\n";
    std::cout << "Settling Time: " << best.metrics.settlingTime << " s\n";
    std::cout << "Steady-State Error: " << best.metrics.steadyStateError << " m\n";
    std::cout << "Score: " << best.metrics.score << "\n";

    // -----------------------------
    // Save CSV
    // -----------------------------
    std::ofstream file("output/pid_tuning_results.csv");

    file << "kp,ki,kd,overshoot,settling_time,sse,score\n";

    for (const auto &r : allResults)
    {
        file << r.kp << ","
             << r.ki << ","
             << r.kd << ","
             << r.metrics.overshootPercent << ","
             << r.metrics.settlingTime << ","
             << r.metrics.steadyStateError << ","
             << r.metrics.score << "\n";
    }

    file.close();

    std::cout << "\nResults saved to output/pid_tuning_results.csv\n";

    return 0;
}