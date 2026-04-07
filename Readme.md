# Flight Computer Simulator

A modular C++ flight control simulator that models vertical dynamics and evaluates PID controller performance through simulation, metrics analysis, and visualization.

## Overview

This project simulates a 1D vertical flight system and implements a PID-based flight computer to track a target altitude. It is designed with a clean, scalable architecture to support control system experimentation, tuning, and analysis.

### Core Capabilities

- Physics-based altitude simulation
- PID controller with configurable gains
- Automated performance metrics:
  - Overshoot
  - Settling time
  - Steady-state error
- CSV output for analysis
- Python-based visualization pipeline

## Project Structure

project/
├── CMakeLists.txt
├── include/project/
│ ├── control/ # Flight control logic (PID, FlightComputer)
│ ├── physics/ # State and system dynamics
│ ├── sensors/ # Sensor abstraction
│ └── common/ # Shared types, constants, metrics
├── src/
│ ├── control/
│ ├── physics/
│ └── sensors/
├── tests/ # Unit tests
├── apps/
│ └── simulator_main.cpp
├── output/ # Generated simulation CSVs
└── scripts/
└── plot.py # Visualization

## 🚀 Build Instructions

### 1. Prerequisites

- CMake ≥ 3.16
- C++17 compatible compiler (MSVC / GCC / Clang)
- Python (optional, for plotting)

### 2. Build the Project

```bash
cmake -S . -B build
cmake --build build
```

```powershell
cmake -S . -B build `
  -G "Unix Makefiles" `
  -DCMAKE_C_COMPILER="C:/Users/JamesPN/VSCode/msys2/mingw64/bin/gcc.exe" `
  -DCMAKE_CXX_COMPILER="C:/Users/JamesPN/VSCode/msys2/mingw64/bin/g++.exe"
cmake --build build
```

This will:

- Compile the simulator
- Build unit tests
- Output binaries to:
  - `build/bin/` (simulator)
  - `build/tests/` (tests)

### 3. Run the Simulator

```bash
./build/bin/simulator
```

Or with custom PID parameters:

```bash
./build/bin/simulator <kp> <ki> <kd>
```

Example:

```bash
./build/bin/simulator 0.3 0.01 0.15
```

This will generate CSV output in:

```
output/
```

### 4. Run Tests

```bash
ctest --test-dir build
```

### 5. (Optional) Visualize Results

```bash
python ./apps/plot.py
```

This will:

- Run multiple PID configurations
- Plot altitude response comparisons

### 6. Full Auto Workflow

```powershell
$ErrorActionPreference = "Stop"

Write-Host "Cleaning build directory..."
if (Test-Path build) {
    Remove-Item -Recurse -Force build
}

Write-Host "Configuring project..."
cmake -S . -B build `
  -G "Unix Makefiles" `
  -DCMAKE_C_COMPILER="C:/Users/JamesPN/VSCode/msys2/mingw64/bin/gcc.exe" `
  -DCMAKE_CXX_COMPILER="C:/Users/JamesPN/VSCode/msys2/mingw64/bin/g++.exe"

Write-Host "Building project..."
cmake --build build

Write-Host "Running simulation..."
& "$PSScriptRoot\build\bin\simulator.exe"

$new_env = $false
Write-Host "Activating virtual environment..."
if (!(Test-Path ".venv")) {
    $new_env = $true
    Write-Host "Creating virtual environment..."
    py -m venv .venv
}
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\.venv\Scripts\Activate.ps1
if ($new_env) {
    Write-Host "Installing dependencies..."
    py -m pip install -r requirement.txt
}

write-Host "Generating plot..."
python ./apps/plot.py

Write-Host "Done!"
```

## 🧠 Metrics & Evaluation

Each simulation run is evaluated using:

- Overshoot (%)
- Settling Time (seconds)
- Steady-State Error

## 🛠 Tech Stack

- C++ (core simulation)
- Python (visualization)
- CSV (data logging)

## 📌 Next Steps

- Automated PID tuning (grid search or optimization)
- Batch simulation + scoring
- Real-time visualization
- Performance benchmarking table

## Why This Project Matters

This project demonstrates:

- Strong C++ system design fundamentals
- Control systems understanding
- Clean separation of responsibilities
- Data-driven engineering workflow

It reflects patterns used in real-world aerospace and robotics systems.
