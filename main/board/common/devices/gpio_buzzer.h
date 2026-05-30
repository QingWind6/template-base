#pragma once

#include "buzzer.h"

#include "driver/gpio.h"

class GpioBuzzer : public Buzzer {
public:
    explicit GpioBuzzer(gpio_num_t pin, bool active_high = true);

    esp_err_t Initialize() override;
    esp_err_t Set(bool on) override;

private:
    gpio_num_t pin_;
    bool active_high_;
};
