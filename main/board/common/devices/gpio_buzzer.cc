#include "gpio_buzzer.h"

#include "board_gpio.h"

GpioBuzzer::GpioBuzzer(gpio_num_t pin, bool active_high) : pin_(pin), active_high_(active_high) {}

esp_err_t GpioBuzzer::Initialize() {
    return BoardConfigureOutput(pin_, active_high_ ? false : true);
}

esp_err_t GpioBuzzer::Set(bool on) {
    return BoardSetGpioLevel(pin_, active_high_ ? on : !on);
}
