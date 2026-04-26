# Battery SoC Estimator 

A C++ implementation of a real-time Battery State of Charge (SoC) estimator using the Coulomb Counting method with charge/discharge efficiency modeling.

This project is a C++ counterpart to my MATLAB/Simulink BESS modeling work, reimplementing core battery management logic in a systems programming context.

## What It Does

- Reads timestamped current and voltage telemetry from a CSV file
- Tracks SoC% over time using Coulomb counting with configurable efficiency factor
- Detects and flags battery status: `NORMAL`, `LOW_BATTERY`, `NEAR_FULL`, `FULLY_CHARGED`, `DEPLETED`
- Logs all output to `output/soc_log.csv` for post-processing

## How Coulomb Counting Works

SoC is estimated by integrating current over time:

```
delta_charge = current (A) * dt (hours)
SoC += (delta_charge / capacity_Ah) * 100%
```

Charging and discharging apply efficiency in opposite directions, modeling real-world energy loss.

## Project Structure

```
soc_estimator/
├── main.cpp          # Entry point, reads CSV, drives simulation loop
├── battery.h/.cpp    # Battery class: SoC update, status logic
├── csv_reader.h/.cpp # Parses telemetry CSV, computes dt between samples
├── data/
│   └── battery_data.csv   # Sample charge/discharge cycle data
├── output/
│   └── soc_log.csv        # Generated output log
└── Makefile
```

## Build and Run

**Requirements:** g++ with C++17 support

```bash
make run
```

Or manually:
```bash
make
./soc_estimator data/battery_data.csv
```

## Custom Input

The input CSV must follow this format:

```
time_s,voltage_V,current_A
0,3.80,5.0
10,3.82,5.0
...
```

- Positive current = charging
- Negative current = discharging
- Time column is used to compute `dt` between samples automatically

## Battery Configuration

Configured in `main.cpp`:

```cpp
Battery bat(
    100.0,   // capacity in Ah
    30.0,    // initial SoC %
    4.2,     // max voltage (overcharge threshold)
    2.8,     // min voltage (deep discharge threshold)
    0.95     // round-trip efficiency
);
```

## Sample Output

```
Time(s) | Voltage(V) | Current(A) | SoC(%) | Status
------------------------------------------------------------
   0.00 |       3.80 |       5.00 |  30.00 | NORMAL
  60.00 |       3.92 |       5.00 |  30.08 | NORMAL
 720.00 |       4.20 |       0.00 |  30.64 | NORMAL
 780.00 |       4.20 |      -8.00 |  30.50 | NORMAL
```

## Concepts Demonstrated

- OOP with encapsulated Battery state
- File I/O with CSV parsing
- Struct-based data modeling (`TelemetryPoint`)
- Physical modeling: Coulomb counting, efficiency factors, SoC clamping
- Separation of concerns across multiple translation units
