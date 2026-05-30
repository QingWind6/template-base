#pragma once

#include "display.h"

#include "driver/gpio.h"

class GpioDisplay : public Display {
public:
    GpioDisplay(gpio_num_t sck, gpio_num_t mosi, gpio_num_t cs, gpio_num_t dc, gpio_num_t rst, gpio_num_t busy);

    esp_err_t Initialize() override;

private:
    gpio_num_t sck_;
    gpio_num_t mosi_;
    gpio_num_t cs_;
    gpio_num_t dc_;
    gpio_num_t rst_;
    gpio_num_t busy_;
};
