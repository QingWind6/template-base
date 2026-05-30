#pragma once

#include "esp_err.h"
#include "power_managed.h"

class Buzzer : public PowerManaged {
public:
    ~Buzzer() override = default;

    virtual esp_err_t Initialize() = 0;
    virtual esp_err_t Set(bool on) = 0;
};
