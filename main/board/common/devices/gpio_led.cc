#include "gpio_led.h"

#include "board_gpio.h"

GpioLed::GpioLed(gpio_num_t pin, bool active_high) : pin_(pin), active_high_(active_high) {}

esp_err_t GpioLed::Initialize() {
    return BoardConfigureOutput(pin_, active_high_ ? false : true);
}

esp_err_t GpioLed::Set(bool on) {
    is_on_ = on;
    return BoardSetGpioLevel(pin_, active_high_ ? on : !on);
}

esp_err_t GpioLed::Toggle() {
    return Set(!is_on_);
}
