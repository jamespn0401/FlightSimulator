import matplotlib.pyplot as plt
import pandas as pd

import os
import platform
import subprocess
import matplotlib

# Detect headless Linux before importing pyplot to avoid backend issues
# Testing CI/CD full run


def is_headless() -> bool:
    return platform.system() == "Linux" and os.environ.get("DISPLAY", "") == ""


if is_headless():
    matplotlib.use("Agg")


def finalize_plot(output_path: str) -> None:
    if is_headless():
        plt.savefig(output_path, bbox_inches="tight")
        print(f"Saved: {output_path}")
        plt.close()


def main() -> None:
    os.makedirs("output", exist_ok=True)

    configs = [
        (0.3, 0.02, 0.5),
    ]

    exe = "simulator.exe" if platform.system() == "Windows" else "simulator"
    simulator_path = os.path.join("build", "bin", exe)

    if not os.path.exists(simulator_path):
        raise FileNotFoundError(
            f"Simulator not found: {simulator_path}\n"
            "Build the project first with:\n"
            "  cmake -S . -B build\n"
            "  cmake --build build"
        )

    results = []

    for kp, ki, kd in configs:
        print(f"Running kp={kp}, ki={ki}, kd={kd}")

        subprocess.run(
            [simulator_path, str(kp), str(ki), str(kd)],
            check=True
        )

        csv_path = f"output/simulation_kp_{kp:.2f}_ki_{ki:.2f}_kd_{kd:.2f}.csv"

        if not os.path.exists(csv_path):
            raise FileNotFoundError(f"Expected CSV not found: {csv_path}")

        df = pd.read_csv(csv_path)
        results.append((f"Kp={kp}, Ki={ki}, Kd={kd}", df, kp, ki, kd))

    # Plot 1: altitude vs thrust for each CSV
    for label, df, kp, ki, kd in results:
        fig, ax1 = plt.subplots()

        ax1.plot(df["time_s"], df["altitude_m"], label="Altitude")
        ax1.plot(
            df["time_s"],
            df["target_altitude_m"],
            linestyle="--",
            label="Target Altitude"
        )
        ax1.set_xlabel("Time (s)")
        ax1.set_ylabel("Altitude (m)")
        ax1.grid(True)

        ax2 = ax1.twinx()
        ax2.plot(
            df["time_s"],
            df["thrust_command_mps2"],
            linestyle="-.",
            label="Thrust"
        )
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

        output_path = (
            f"output/altitude_vs_thrust_kp_{kp:.2f}_ki_{ki:.2f}_kd_{kd:.2f}.png"
        )
        finalize_plot(output_path)

    # Plot 2: thrust vs altitude for each CSV
    for label, df, kp, ki, kd in results:
        plt.figure()

        scatter = plt.scatter(
            df["altitude_m"],
            df["thrust_command_mps2"],
            c=df["time_s"],
            s=8
        )
        plt.colorbar(scatter, label="Time (s)")

        plt.xlabel("Altitude (m)")
        plt.ylabel("Thrust Command (m/s^2)")
        plt.title(f"Thrust vs Altitude - {label}")
        plt.grid(True)
        plt.tight_layout()

        output_path = (
            f"output/thrust_vs_altitude_kp_{kp:.2f}_ki_{ki:.2f}_kd_{kd:.2f}.png"
        )
        finalize_plot(output_path)

    if not is_headless():
        plt.show()


if __name__ == "__main__":
    main()
