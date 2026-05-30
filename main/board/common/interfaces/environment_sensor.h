#pragma once

#include "power_managed.h"

struct EnvironmentReading {
    bool has_temperature = false;
    float temperature_c = 0.0f;
    bool has_humidity = false;
    float humidity_percent = 0.0f;
    bool has_pressure = false;
    float pressure_hpa = 0.0f;
};

class EnvironmentSensor : public PowerManaged {
public:
    ~EnvironmentSensor() override = default;

    virtual bool Read(EnvironmentReading* reading) = 0;
};
