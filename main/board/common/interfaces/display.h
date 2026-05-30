#pragma once

#include "esp_err.h"
#include "power_managed.h"

class Display : public PowerManaged {
public:
    ~Display() override = default;

    virtual esp_err_t Initialize() = 0;
};
