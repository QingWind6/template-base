#pragma once

#include "led.h"

#include "driver/gpio.h"

class GpioLed : public Led {
public:
    explicit GpioLed(gpio_num_t pin, bool active_high = true);

    esp_err_t Initialize() override;
    esp_err_t Set(bool on) override;
    esp_err_t Toggle() override;
    bool IsOn() const override {
        return is_on_;
    }

private:
    gpio_num_t pin_;
    bool active_high_;
    bool is_on_ = false;
};
