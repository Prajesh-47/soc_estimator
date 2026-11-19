# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

A small C++17 battery State of Charge (SoC) estimator using Coulomb counting with charge/discharge efficiency. It is the C++ counterpart to the author's MATLAB/Simulink BESS modeling work. There is no linter config.

## Commands

```bash
make            # build ./soc_estimator (g++ -std=c++17 -Wall -Wextra -O2); also creates output/
make run        # build and run against data/battery_data.csv
./soc_estimator path/to/input.csv   # run on custom telemetry
make test       # golden-file diff of output/soc_log.csv, then unit tests
make tests/test_battery && ./tests/test_battery   # unit tests only
make clean      # remove objects, binaries, and output/soc_log.csv
```

Run the binary from the project root. The output path `output/soc_log.csv` is relative to the working directory, and so is the default input.

`make test` runs the sample data and diffs the result against `tests/golden/soc_log.csv`. Any change to the CSV output (columns, values, formatting) makes it fail on purpose. Regenerate the golden file with `cp output/soc_log.csv tests/golden/soc_log.csv` only after reviewing the diff. Unit tests in `tests/test_battery.cpp` use a small `check()` helper (no framework) and pass `dt_s = 0` to keep SoC fixed.

Header dependencies are listed by hand in the Makefile. If you add a new header, add a prerequisite line for it.

## Architecture

The data flow is linear: `readCSV` → loop over `TelemetryPoint`s → `Battery::update` → `Battery::getStatus` → stdout table plus `output/soc_log.csv`.

- **`csv_reader`**: parses `time_s,voltage_V,current_A` (skips the header) and precomputes each point's `dt` from consecutive timestamps. The first sample is given `dt = 1.0`, so SoC moves slightly at t=0. There is no input validation: a blank or malformed line makes `std::stod` throw.
- **`Battery`**: holds SoC state. `update()` integrates `current * dt/3600` against capacity. Positive current means charging and is multiplied by `efficiency`. Negative current means discharging and is divided by it. SoC is clamped to [0, 100]. Status comes only from SoC thresholds (`SOC_LOW_WARN`=10, `SOC_HIGH_WARN`=95, constants in `battery.h`). `update()` also keeps the last `CURRENT_AVG_WINDOW` (5) currents. `getTimeRemainingMin()` uses their plain average (not time-weighted) to estimate time to 0% or 100%, applying efficiency the same way `update()` does. It returns `std::nullopt` when |average| < 0.1 A, which becomes an empty `time_remaining_min` CSV field.
- **`main.cpp`**: hardcodes the battery config (`Battery bat(100.0, 30.0, 4.2, 2.8, 0.95)`: capacity in Ah, initial SoC %, max V, min V, efficiency) and does all the output formatting.

`max_voltage` and `min_voltage` are stored in `Battery` but never used. Voltage from the CSV is only passed through to the output and has no effect on SoC or status. The README's status list (`NORMAL`, `LOW_BATTERY`, `NEAR_FULL`, `FULLY_CHARGED`, `DEPLETED`) matches `getStatus()`.
