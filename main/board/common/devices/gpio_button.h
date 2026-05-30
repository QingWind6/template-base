#pragma once

#include "button.h"

#include "driver/gpio.h"

class GpioButton : public Button {
public:
    explicit GpioButton(gpio_num_t pin, bool active_low = true);

    esp_err_t Initialize() override;
    bool IsPressed() const override;

private:
    gpio_num_t pin_;
    bool active_low_;
};
