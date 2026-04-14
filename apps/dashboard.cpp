#include <vector>
#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "implot.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "project/control/FlightComputer.hpp"
#include "project/physics/State.hpp"
#include "project/common/Telemetry.hpp"
#include "project/common/TelemetryBuffer.hpp"
#include "project/dashboard/Utils.hpp"
#include "project/dashboard/Theme.hpp"

double SIM_TIME = 0.0;

namespace
{
    void glfwErrorCallback(int error, const char *description)
    {
        std::cerr << "GLFW Error " << error << ": " << description << '\n';
    }

    bool SliderDouble(const char *label, double &value, double min, double max)
    {
        float valueFloat = static_cast<float>(value);
        bool changed = ImGui::SliderFloat(label, &valueFloat, static_cast<float>(min), static_cast<float>(max));
        value = static_cast<double>(valueFloat);
        return changed;
    }

    void controlUI(project::dashboard::ControlPanel &controlPanel)
    {
        project::dashboard::TextHeading("Simulation Controls");
        SliderDouble("Kp", controlPanel.kp, 0.0, 2.0);
        SliderDouble("Ki", controlPanel.ki, 0.0, 1.0);
        SliderDouble("Kd", controlPanel.kd, 0.0, 1.0);
        SliderDouble("Target Altitude (m)", controlPanel.targetAltitude, 0.0, 500.0);
        SliderDouble("dt (s)", controlPanel.dt, 0.001, 0.05);

        if (ImGui::Button("Start / Restart"))
        {
            controlPanel.state = {};
            controlPanel.telemetry.clear();
            SIM_TIME = 0.0;
            controlPanel.paused = false;
            controlPanel.running = true;
            controlPanel.flightComputer = project::control::FlightComputer(controlPanel.kp, controlPanel.ki, controlPanel.kd, controlPanel.minThrust, controlPanel.maxThrust);
        }

        ImGui::SameLine();

        if (ImGui::Button(controlPanel.paused ? "Resume" : "Pause"))
        {
            controlPanel.paused = !controlPanel.paused;
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            controlPanel.state = {};
            controlPanel.telemetry.clear();
            SIM_TIME = 0.0;
            controlPanel.paused = false;
            controlPanel.running = false;
            controlPanel.flightComputer = project::control::FlightComputer(controlPanel.kp, controlPanel.ki, controlPanel.kd, controlPanel.minThrust, controlPanel.maxThrust);
        }

        auto points = controlPanel.telemetry.snapshot();

        double altitude = points.empty() ? 0.0 : points.back().altitudeMeters;
        double velocity = points.empty() ? 0.0 : points.back().velocityMetersPerSecond;
        double accel = points.empty() ? 0.0 : points.back().accelerationMetersPerSecondSquared;
        double thrust = points.empty() ? 0.0 : points.back().thrustCommandMetersPerSecondSquared;
        double error = points.empty() ? 0.0 : points.back().altitudeErrorMeters;

        ImGui::Separator();
        ImGui::Text("Time: %.2f s", SIM_TIME);
        ImGui::Text("Altitude: %.2f m", altitude);
        ImGui::Text("Velocity: %.2f m/s", velocity);
        ImGui::Text("Acceleration: %.2f m/s^2", accel);
        ImGui::Text("Thrust Command: %.2f m/s^2", thrust);
        ImGui::Text("Altitude Error: %.2f m", error);
    }

    void graphUI(project::dashboard::ControlPanel &controlPanel)
    {
        auto points = controlPanel.telemetry.snapshot();
        std::vector<double> xs, altitudeYs, velocityYs, thrustYs, errorYs;
        xs.reserve(points.size());
        altitudeYs.reserve(points.size());
        velocityYs.reserve(points.size());
        thrustYs.reserve(points.size());
        errorYs.reserve(points.size());

        for (const auto &p : points)
        {
            xs.push_back(p.timeSeconds);
            altitudeYs.push_back(p.altitudeMeters);
            velocityYs.push_back(p.velocityMetersPerSecond);
            thrustYs.push_back(p.thrustCommandMetersPerSecondSquared);
            errorYs.push_back(p.altitudeErrorMeters);
        }

        project::dashboard::render("Altitude", xs, altitudeYs, controlPanel);
        project::dashboard::render("Velocity", xs, velocityYs, controlPanel);
        project::dashboard::render("Thrust Command", xs, thrustYs, controlPanel);
        project::dashboard::render("Altitude Error", xs, errorYs, controlPanel);
    }
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    const char *glslVersion = "#version 130";
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *monitorMode = glfwGetVideoMode(primaryMonitor);
    GLFWwindow *window = glfwCreateWindow(monitorMode->width, monitorMode->height, "Flight Simulator Dashboard", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    project::dashboard::ControlPanel controlPanel{};

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (controlPanel.running && !controlPanel.paused)
        {
            const auto result = controlPanel.flightComputer.step(controlPanel.state, controlPanel.targetAltitude, controlPanel.dt, SIM_TIME);
            const double error = controlPanel.targetAltitude - result.sensorData.altitudeMeters;

            controlPanel.telemetry.push({result.timeSeconds,
                                         result.sensorData.altitudeMeters,
                                         result.sensorData.velocityMetersPerSecond,
                                         result.sensorData.accelerationMetersPerSecondSquared,
                                         result.thrustCommandMetersPerSecondSquared,
                                         controlPanel.targetAltitude,
                                         error});
            SIM_TIME += controlPanel.dt;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        float totalWidth = ImGui::GetContentRegionAvail().x;
        float leftWidth = totalWidth * 0.2f;

        // Controls panel
        ImGui::BeginChild("Controls", ImVec2(leftWidth, 0), true);
        controlUI(controlPanel);
        ImGui::EndChild();

        // Move to the right side
        ImGui::SameLine();

        // Graphs panel
        ImGui::BeginChild("Graphs", ImVec2(0, 0), true);
        graphUI(controlPanel);
        ImGui::EndChild();

        ImGui::Render();
        int displayW = 0;
        int displayH = 0;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}