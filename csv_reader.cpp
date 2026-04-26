#include "csv_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<TelemetryPoint> readCSV(const std::string& filename) {
    std::vector<TelemetryPoint> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open: " << filename << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line);

    double prev_time = 0.0;
    bool first = true;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        TelemetryPoint pt;

        std::getline(ss, token, ','); pt.time    = std::stod(token);
        std::getline(ss, token, ','); pt.voltage = std::stod(token);
        std::getline(ss, token, ','); pt.current = std::stod(token);

        pt.dt = first ? 1.0 : pt.time - prev_time;
        prev_time = pt.time;
        first = false;

        data.push_back(pt);
    }

    return data;
}
