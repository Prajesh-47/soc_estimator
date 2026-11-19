#pragma once
#include <cstddef>
#include <deque>
#include <optional>
#include <string>

class Battery {
public:
    Battery(double capacity_Ah, double initial_soc_pct,
            double max_voltage, double min_voltage, double efficiency);

    void update(double current_A, double dt_s);
    double getSoC() const;
    std::string getStatus() const;
    std::optional<double> getTimeRemainingMin() const;

private:
    double capacity_Ah;
    double soc;
    double max_voltage;
    double min_voltage;
    double efficiency;
    std::deque<double> recent_currents;

    static constexpr double SOC_HIGH_WARN = 95.0;
    static constexpr double SOC_LOW_WARN  = 10.0;
    static constexpr double SOC_MAX       = 100.0;
    static constexpr double SOC_MIN       = 0.0;

    static constexpr std::size_t CURRENT_AVG_WINDOW = 5;
    static constexpr double MIN_ESTIMATE_CURRENT    = 0.1;
};
