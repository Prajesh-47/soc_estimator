#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <optional>
#include "battery.h"
#include "csv_reader.h"

int main(int argc, char* argv[]) {
    std::string filename = "data/battery_data.csv";
    if (argc > 1) filename = argv[1];

    Battery bat(100.0, 30.0, 4.2, 2.8, 0.95);

    std::vector<TelemetryPoint> data = readCSV(filename);
    if (data.empty()) {
        std::cerr << "Error: Could not read data from " << filename << std::endl;
        return 1;
    }

    std::ofstream out("output/soc_log.csv");
    out << "time_s,voltage_V,current_A,soc_pct,status,time_remaining_min\n";

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Time(s) | Voltage(V) | Current(A) | SoC(%) | Status\n";
    std::cout << std::string(60, '-') << "\n";

    for (const auto& point : data) {
        bat.update(point.current, point.dt);
        std::string status = bat.getStatus();
        std::optional<double> remaining = bat.getTimeRemainingMin();

        std::cout << std::setw(7) << point.time << " | "
                  << std::setw(10) << point.voltage << " | "
                  << std::setw(10) << point.current << " | "
                  << std::setw(6) << bat.getSoC() << " | "
                  << status << "\n";

        out << point.time << "," << point.voltage << ","
            << point.current << "," << bat.getSoC() << ","
            << status << ",";
        if (remaining) out << *remaining;
        out << "\n";
    }

    std::optional<double> final_remaining = bat.getTimeRemainingMin();
    std::cout << "\nFinal: SoC " << bat.getSoC() << "% | Status "
              << bat.getStatus() << " | Time remaining: ";
    if (final_remaining) std::cout << *final_remaining << " min\n";
    else                 std::cout << "N/A\n";

    out.close();
    std::cout << "\nLog saved to output/soc_log.csv\n";
    return 0;
}
