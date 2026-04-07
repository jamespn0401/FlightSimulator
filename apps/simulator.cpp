#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>

#include "project/control/FlightComputer.hpp"
#include "project/physics/State.hpp"
#include "project/common/Functions.hpp"

int main(int argc, char *argv[])
{
    double kp = 0.25;
    double ki = 0.01;
    double kd = 0.15;
    double minThrustAcceleration = 0.0;
    double maxThrustAcceleration = 20.0;

    if (argc == 4)
    {
        kp = std::atof(argv[1]);
        ki = std::atof(argv[2]);
        kd = std::atof(argv[3]);
    }
    else
    {
        std::cout << "Using default PID parameters: "
                  << "kp=" << kp << ", "
                  << "ki=" << ki << ", "
                  << "kd=" << kd << "\n";
        std::cout << "To specify custom PID parameters, run the program with: "
                  << "./simulator <kp> <ki> <kd>\n";
    }

    std::string filename = "simulation_kp_" + project::common::to_string_precision(kp, 2) +
                           "_ki_" + project::common::to_string_precision(ki, 2) +
                           "_kd_" + project::common::to_string_precision(kd, 2) + ".csv";

    const std::string outputDir = "output";
    std::filesystem::create_directories(outputDir);

    std::ofstream csvFile(outputDir + "/" + filename);
    if (!csvFile.is_open())
    {
        std::cerr << "Failed to open output file for writing.\n";
        return 1;
    }
    csvFile << "time_s,altitude_m,velocity_mps,acceleration_mps2,thrust_command_mps2,state_altitude_m,target_altitude_m,altitude_error_m\n";

    constexpr project::common::Scalar kDtSeconds = 0.01;
    constexpr project::common::Scalar kSimulationDurationSeconds = 30.0;
    constexpr project::common::Scalar kTargetAltitudeMeters = 100.0;

    project::physics::State state{0.0, 0.0};

    project::control::FlightComputer flightComputer(
        kp,
        ki,
        kd,
        minThrustAcceleration,
        maxThrustAcceleration);

    std::cout << std::fixed << std::setprecision(2);

    for (project::common::Scalar timeSeconds = 0.0;
         timeSeconds <= kSimulationDurationSeconds;
         timeSeconds += kDtSeconds)
    {
        const project::control::StepResult result =
            flightComputer.step(
                state,
                kTargetAltitudeMeters,
                kDtSeconds,
                timeSeconds);

        const project::common::Scalar altitudeError =
            kTargetAltitudeMeters - result.sensorData.altitudeMeters;

        csvFile << result.timeSeconds << ","
                << result.sensorData.altitudeMeters << ","
                << result.sensorData.velocityMetersPerSecond << ","
                << result.sensorData.accelerationMetersPerSecondSquared << ","
                << result.thrustCommandMetersPerSecondSquared << ","
                << state.altitudeMeters << ","
                << kTargetAltitudeMeters << ","
                << altitudeError << "\n";
    }
    csvFile.close();
    std::cout << "Simulation complete. Output written to simulation_output.csv\n";
    return 0;
}