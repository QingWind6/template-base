#include "gpio_button.h"

#include "board_gpio.h"

GpioButton::GpioButton(gpio_num_t pin, bool active_low) : pin_(pin), active_low_(active_low) {}

esp_err_t GpioButton::Initialize() {
    return BoardConfigureInput(pin_, active_low_, !active_low_);
}

bool GpioButton::IsPressed() const {
    const bool level = BoardGetGpioLevel(pin_);
    return active_low_ ? !level : level;
}
