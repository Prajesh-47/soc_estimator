#include "battery.h"
#include <cmath>

Battery::Battery(double capacity_Ah, double initial_soc_pct,
                 double max_voltage, double min_voltage, double efficiency)
    : capacity_Ah(capacity_Ah),
      soc(initial_soc_pct),
      max_voltage(max_voltage),
      min_voltage(min_voltage),
      efficiency(efficiency) {}

void Battery::update(double current_A, double dt_s) {
    double delta_charge = current_A * (dt_s / 3600.0);

    if (current_A > 0) {
        soc += (delta_charge / capacity_Ah) * 100.0 * efficiency;
    } else {
        soc += (delta_charge / capacity_Ah) * 100.0 / efficiency;
    }

    if (soc > SOC_MAX) soc = SOC_MAX;
    if (soc < SOC_MIN) soc = SOC_MIN;

    recent_currents.push_back(current_A);
    if (recent_currents.size() > CURRENT_AVG_WINDOW) {
        recent_currents.pop_front();
    }
}

std::optional<double> Battery::getTimeRemainingMin() const {
    if (recent_currents.empty()) return std::nullopt;

    double sum = 0.0;
    for (double c : recent_currents) sum += c;
    double avg_current = sum / recent_currents.size();

    if (std::abs(avg_current) < MIN_ESTIMATE_CURRENT) return std::nullopt;

    double hours;
    if (avg_current > 0) {
        // Charging: SoC rises at I * efficiency / capacity
        hours = ((SOC_MAX - soc) / 100.0) * capacity_Ah / (avg_current * efficiency);
    } else {
        // Discharging: SoC falls at |I| / (capacity * efficiency)
        hours = ((soc - SOC_MIN) / 100.0) * capacity_Ah * efficiency / -avg_current;
    }

    return hours * 60.0;
}

double Battery::getSoC() const {
    return soc;
}

std::string Battery::getStatus() const {
    if (soc >= SOC_MAX)        return "FULLY_CHARGED";
    if (soc >= SOC_HIGH_WARN)  return "NEAR_FULL";
    if (soc <= SOC_MIN)        return "DEPLETED";
    if (soc <= SOC_LOW_WARN)   return "LOW_BATTERY";
    return "NORMAL";
}
