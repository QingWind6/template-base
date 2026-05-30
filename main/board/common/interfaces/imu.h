#pragma once

#include "power_managed.h"

struct ImuReading {
    float accel_x = 0.0f;
    float accel_y = 0.0f;
    float accel_z = 0.0f;
    float gyro_x = 0.0f;
    float gyro_y = 0.0f;
    float gyro_z = 0.0f;
    bool has_temperature = false;
    float temperature_c = 0.0f;
};

class Imu : public PowerManaged {
public:
    ~Imu() override = default;

    virtual bool Read(ImuReading* reading) = 0;
};
