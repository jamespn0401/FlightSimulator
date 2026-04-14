# ==========================================
# Project Configuration
# ==========================================

cmake_minimum_required(VERSION 3.16)

project(FlightComputerSimulator LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Copy assets (fonts) to build/bin
file(COPY ${CMAKE_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# ==========================================
# Fetch and Configure External Libraries
# ==========================================
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)

FetchContent_Declare(
    implot
    GIT_REPOSITORY https://github.com/epezent/implot.git
    GIT_TAG master
)

FetchContent_MakeAvailable(glfw imgui implot)

# ==========================================
# Project Source Files and Targets
# ==========================================
add_library(imgui_lib
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)
target_include_directories(imgui_lib
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${implot_SOURCE_DIR}
)

add_library(glad 
    external/glad/src/glad.c
)
target_include_directories(glad 
    PUBLIC 
        external/glad/include
)

add_library(dashboard_lib
    src/dashboard/Utils.cpp
    src/dashboard/Theme.cpp
)
target_include_directories(dashboard_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(project_lib
    src/control/PIDController.cpp
    src/control/FlightComputer.cpp
    src/physics/Dynamics.cpp
    src/sensors/Sensor.cpp
    src/common/Functions.cpp
    src/common/Metrics.cpp
    src/common/TelemetryBuffer.cpp
)
target_include_directories(project_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# ==========================================
# Applications
# ==========================================
target_link_libraries(imgui_lib
    PUBLIC
        glfw
)
target_link_libraries(dashboard_lib
    PUBLIC
        imgui_lib
        project_lib
)

add_executable(simulator
    apps/simulator.cpp
)
target_link_libraries(simulator
    PRIVATE
        project_lib
)
add_executable(pid_tuner
    apps/pid_tuner.cpp
)
target_link_libraries(pid_tuner
    PRIVATE
        project_lib
)
add_executable(dashboard
    apps/dashboard.cpp
)
target_link_libraries(dashboard
    PRIVATE
        imgui_lib
        glad
        glfw
        project_lib
        dashboard_lib
)

# ==========================================
# Unit Tests
# ==========================================
add_executable(pid_controller_tests
    tests/PIDControllerTests.cpp
)

set_target_properties(pid_controller_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_link_libraries(pid_controller_tests
    PRIVATE
        project_lib
)

add_executable(dynamics_tests
    tests/DynamicsTests.cpp
)

set_target_properties(dynamics_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR}
)

target_link_libraries(dynamics_tests
    PRIVATE
        project_lib
)

enable_testing()

add_test(NAME PIDControllerTests COMMAND pid_controller_tests)
add_test(NAME DynamicsTests COMMAND dynamics_tests)

# ==========================================
# Enable Strict Warnings for All Targets
# ==========================================
function(enable_strict_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    )
endfunction()
enable_strict_warnings(project_lib)
enable_strict_warnings(simulator)
enable_strict_warnings(pid_tuner)
enable_strict_warnings(pid_controller_tests)
enable_strict_warnings(dynamics_tests)