#pragma once
#include <string>

class Battery {
public:
    Battery(double capacity_Ah, double initial_soc_pct,
            double max_voltage, double min_voltage, double efficiency);

    void update(double current_A, double dt_s);
    double getSoC() const;
    std::string getStatus() const;

private:
    double capacity_Ah;
    double soc;
    double max_voltage;
    double min_voltage;
    double efficiency;

    static constexpr double SOC_HIGH_WARN = 95.0;
    static constexpr double SOC_LOW_WARN  = 10.0;
    static constexpr double SOC_MAX       = 100.0;
    static constexpr double SOC_MIN       = 0.0;
};
