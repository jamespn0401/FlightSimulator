from unittest.mock import patch
import pandas as pd
import plot


def make_fake_df():
    return pd.DataFrame({
        "time_s": [0.0, 1.0, 2.0],
        "altitude_m": [0.0, 10.0, 18.0],
        "target_altitude_m": [20.0, 20.0, 20.0],
        "thrust_command_mps2": [5.0, 3.0, 1.0],
    })


@patch("plot.subprocess.run")
@patch("plot.pd.read_csv")
@patch("plot.finalize_plot")
@patch("plot.os.makedirs")
@patch("plot.os.path.exists")
def test_main_happy_path(
    mock_exists,
    mock_makedirs,
    mock_finalize_plot,
    mock_read_csv,
    mock_subprocess_run
):
    def exists_side_effect(path):
        path = str(path)
        if path.endswith("build/bin/simulator") or path.endswith("build\\bin\\simulator.exe"):
            return True
        if "simulation_kp_0.30_ki_0.02_kd_0.50.csv" in path:
            return True
        return False

    mock_exists.side_effect = exists_side_effect
    mock_read_csv.return_value = make_fake_df()

    plot.main()

    mock_makedirs.assert_called_once_with("output", exist_ok=True)
    mock_subprocess_run.assert_called_once()
    mock_read_csv.assert_called_once()
    assert mock_finalize_plot.call_count == 2


@patch("plot.os.makedirs")
@patch("plot.os.path.exists", return_value=False)
def test_main_raises_if_simulator_missing(mock_exists, mock_makedirs):
    try:
        plot.main()
        assert False, "Expected FileNotFoundError"
    except FileNotFoundError as exc:
        assert "Simulator not found" in str(exc)


@patch("plot.subprocess.run")
@patch("plot.os.makedirs")
@patch("plot.os.path.exists")
def test_main_raises_if_csv_missing(mock_exists, mock_makedirs, mock_subprocess_run):
    def exists_side_effect(path):
        path = str(path)
        if path.endswith("build/bin/simulator") or path.endswith("build\\bin\\simulator.exe"):
            return True
        return False

    mock_exists.side_effect = exists_side_effect

    try:
        plot.main()
        assert False, "Expected FileNotFoundError"
    except FileNotFoundError as exc:
        assert "Expected CSV not found" in str(exc)


@patch("plot.platform.system", return_value="Linux")
@patch.dict("plot.os.environ", {"DISPLAY": ""}, clear=True)
def test_is_headless_true(mock_system):
    assert plot.is_headless() is True


@patch("plot.platform.system", return_value="Windows")
@patch.dict("plot.os.environ", {}, clear=True)
def test_is_headless_false_on_windows(mock_system):
    assert plot.is_headless() is False
