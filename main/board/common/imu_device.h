#pragma once

#include "low_power_controllable.h"

struct ImuData_t {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float temperature_c;
};

class ImuDevice : public LowPowerControllable {
public:
    virtual ~ImuDevice() = default;
    virtual void init(){}
    virtual void update() {}

    virtual bool getData(ImuData_t& _imu_data)
    {
        (void)_imu_data;
        return false;
    }
};
