cmake_minimum_required(VERSION 3.16)

project(FlightComputerSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)

add_library(project_lib
    src/control/PIDController.cpp
    src/control/FlightComputer.cpp
    src/physics/Dynamics.cpp
    src/sensors/Sensor.cpp
)

target_include_directories(project_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(project_lib
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

add_executable(simulator
    apps/simulator_main.cpp
)

target_link_libraries(simulator
    PRIVATE
        project_lib
)

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