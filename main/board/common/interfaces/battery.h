#pragma once

#include "esp_err.h"
#include "power_managed.h"

class Battery : public PowerManaged {
public:
    ~Battery() override = default;

    virtual esp_err_t Initialize() = 0;
    virtual esp_err_t ReadRaw(int* raw) = 0;
    virtual bool IsCharging() const = 0;
};
