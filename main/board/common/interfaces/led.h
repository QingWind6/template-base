#pragma once

#include "esp_err.h"
#include "power_managed.h"

class Led : public PowerManaged {
public:
    ~Led() override = default;

    virtual esp_err_t Initialize() = 0;
    virtual esp_err_t Set(bool on) = 0;
    virtual esp_err_t Toggle() = 0;
    virtual bool IsOn() const = 0;
};
