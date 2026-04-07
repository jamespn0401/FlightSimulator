# Flight Simulator

This project simulates a simplified aerospace flight computer system, implementing a closed-loop control system to maintain target altitude using PID control.

---

## 🚀 Build & Run

### 1. Prerequisites

- CMake ≥ 3.16
- C++17 compatible compiler (MSVC / GCC / Clang)
- Python (optional, for plotting)

---

### 2. Clone the Repository

```bash
git clone <your-repo-url>
cd <your-repo-folder>
```

---

### 3. Build the Project

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

---

### 4. Run the Simulator

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

---

### 5. Run Tests

```bash
ctest --test-dir build
```

---

### 6. (Optional) Visualize Results

```bash
python ./apps/plot.py
```

This will:

- Run multiple PID configurations
- Plot altitude response comparisons

---

### 7. Full Auto Workflow

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

---

---

## 🧠 System Overview

The system models key avionics concepts including:

- Sensor data acquisition
- State estimation
- Control algorithms
- Actuator response
- Telemetry logging

---

## 🏗️ Architecture

- **Sensor Module**: Simulates altitude, velocity, and acceleration inputs
- **Physics Engine**: Updates system state based on control outputs
- **Controller**: PID-based altitude control
- **Actuator Module**: Applies thrust constraints
- **Telemetry System**: Logs and exports system state

---

## 📊 Example Output

- Stable altitude hold
- Oscillation under poor PID tuning
- Response to disturbances

---

## 🛠 Tech Stack

- C++ (core simulation)
- Python (visualization)
- CSV (data logging)

---

## 📚 What I Learned

- Designing real-time simulation loops
- Implementing PID control systems
- Handling noisy sensor data
- Structuring modular, testable systems

---

## 🔮 Future Improvements

- Kalman filter for state estimation
- Hardware-in-the-loop integration (Arduino/Raspberry Pi)
- Real-time dashboard

---

## 📁 Project Structure

```
project/
├── CMakeLists.txt
├── include/
├── src/
├── tests/
├── apps/
└── output/
```

---

## ⚡ Notes

- Default PID values are used if none are provided
- Output files include PID parameters in the filename
- Simulation runs for ~30 seconds of simulated time

---

-fg

## 📌 Next Steps

- Add automated PID tuning
- Add performance metrics (overshoot, settling time)
- Generate plots automatically after simulation
- Add CI/CD (GitHub Actions)

---

This project is designed to demonstrate control systems, simulation design, and clean C++ architecture for real-time systems.
