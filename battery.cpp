#include "battery.h"

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
