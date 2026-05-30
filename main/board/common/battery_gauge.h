#pragma once

#include <cstdint>

#include "low_power_controllable.h"

enum class BatteryStatus_t {
    None = 0,
    Charging,
    Discharging,
    Low,
    Full,
};

enum class ExternalPowerState {
    Unknown = 0,
    Present,
    Absent,
};

class BatteryGauge : public LowPowerControllable {
public:
    virtual ~BatteryGauge() = default;

    virtual bool ReadVoltageMv(uint16_t& voltage_mv) = 0;
    virtual bool ReadPercent(uint16_t& percent) = 0;
    virtual bool ReadStatus(BatteryStatus_t& status) = 0;
    virtual bool ReadExternalPowerState(ExternalPowerState& state) {
        state = ExternalPowerState::Unknown;
        return false;
    }
};
