#pragma once
#include <vector>
#include <string>

struct TelemetryPoint {
    double time;
    double voltage;
    double current;
    double dt;
};

std::vector<TelemetryPoint> readCSV(const std::string& filename);
