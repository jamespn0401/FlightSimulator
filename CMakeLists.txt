cmake_minimum_required(VERSION 3.16)

project(FlightComputerSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(TEST_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(project_lib
    src/control/PIDController.cpp
    src/control/FlightComputer.cpp
    src/physics/Dynamics.cpp
    src/sensors/Sensor.cpp
    src/common/Functions.cpp
    src/common/Metrics.cpp
)

target_include_directories(project_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(project_lib
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
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