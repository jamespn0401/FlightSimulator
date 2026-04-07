import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import os

configs = [
    (0.3, 0.02, 0.5),
]

exe_path = "build/bin/simulator.exe"
results = []

for kp, ki, kd in configs:
    print(f"Running kp={kp}, ki={ki}, kd={kd}")

    subprocess.run([exe_path, str(kp), str(ki), str(kd)], check=True)

    filename = f"output/simulation_kp_{kp:.2f}_ki_{ki:.2f}_kd_{kd:.2f}.csv"
    df = pd.read_csv(filename)

    results.append((f"Kp={kp}, Ki={ki}, Kd={kd}", df))

# One plot per CSV
for label, df in results:
    fig, ax1 = plt.subplots()

    ax1.plot(df["time_s"], df["altitude_m"], label="Altitude")
    ax1.plot(df["time_s"], df["target_altitude_m"],
             linestyle="--", label="Target Altitude")
    ax1.set_xlabel("Time (s)")
    ax1.set_ylabel("Altitude (m)")
    ax1.grid()

    ax2 = ax1.twinx()
    ax2.plot(df["time_s"], df["thrust_command_mps2"],
             linestyle="-.", color="green", label="Thrust")
    ax2.set_ylabel("Thrust Command (m/s^2)")

    lines1, labels1 = ax1.get_legend_handles_labels()
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(
        lines1 + lines2,
        labels1 + labels2,
        loc="upper center",
        bbox_to_anchor=(0.5, -0.15),
        ncol=3
    )

    plt.title(f"Altitude vs Thrust - {label}")
    plt.tight_layout(rect=[0, 0.05, 1, 1])
    plt.tight_layout()

for label, df in results:
    plt.figure()

    plt.plot(df["altitude_m"], df["thrust_command_mps2"])

    plt.xlabel("Altitude (m)")
    plt.ylabel("Thrust Command (m/s^2)")
    plt.title(f"Thrust vs Altitude - {label}")
    plt.grid()

    plt.scatter(
        df["altitude_m"],
        df["thrust_command_mps2"],
        c=df["time_s"],
        s=5
    )
plt.colorbar(label="Time (s)")

plt.show()
