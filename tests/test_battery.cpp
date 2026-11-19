#include <cmath>
#include <iostream>
#include <string>
#include "battery.h"

static int failures = 0;

static void check(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "FAIL: " << name << std::endl;
        failures++;
    }
}

static bool near(double a, double b) {
    return std::abs(a - b) < 1e-6;
}

// dt_s = 0 keeps SoC fixed so the expected estimate is exact.
static Battery makeBattery() {
    return Battery(100.0, 50.0, 4.2, 2.8, 0.95);
}

int main() {
    {
        Battery bat = makeBattery();
        check(!bat.getTimeRemainingMin().has_value(), "no samples -> no estimate");
    }
    {
        Battery bat = makeBattery();
        for (int i = 0; i < 5; i++) bat.update(0.05, 0.0);
        check(!bat.getTimeRemainingMin().has_value(), "near-zero current -> no estimate");
    }
    {
        Battery bat = makeBattery();
        for (double c : {5.0, -5.0, 5.0, -5.0, 0.2}) bat.update(c, 0.0);
        check(!bat.getTimeRemainingMin().has_value(), "average near zero -> no estimate");
    }
    {
        Battery bat = makeBattery();
        for (int i = 0; i < 5; i++) bat.update(-10.0, 0.0);
        auto t = bat.getTimeRemainingMin();
        check(t.has_value() && near(*t, 285.0), "discharge time-to-empty");
    }
    {
        Battery bat = makeBattery();
        for (int i = 0; i < 5; i++) bat.update(10.0, 0.0);
        auto t = bat.getTimeRemainingMin();
        check(t.has_value() && near(*t, 50.0 / (10.0 * 0.95) * 60.0), "charge time-to-full");
    }
    {
        Battery bat = makeBattery();
        for (int i = 0; i < 5; i++) bat.update(0.0, 0.0);
        bat.update(-10.0, 0.0);
        auto t = bat.getTimeRemainingMin();
        check(t.has_value() && near(*t, 0.5 * 100.0 * 0.95 / 2.0 * 60.0), "window keeps last 5 samples");
    }

    if (failures == 0) std::cout << "All battery tests passed\n";
    return failures == 0 ? 0 : 1;
}
