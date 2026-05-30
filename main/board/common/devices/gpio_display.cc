#include "gpio_display.h"

#include "board_gpio.h"

#include "esp_check.h"

GpioDisplay::GpioDisplay(gpio_num_t sck, gpio_num_t mosi, gpio_num_t cs, gpio_num_t dc, gpio_num_t rst, gpio_num_t busy)
    : sck_(sck), mosi_(mosi), cs_(cs), dc_(dc), rst_(rst), busy_(busy) {}

esp_err_t GpioDisplay::Initialize() {
    ESP_RETURN_ON_ERROR(BoardConfigureOutput(sck_), "GpioDisplay", "sck init failed");
    ESP_RETURN_ON_ERROR(BoardConfigureOutput(mosi_), "GpioDisplay", "mosi init failed");
    ESP_RETURN_ON_ERROR(BoardConfigureOutput(cs_, true), "GpioDisplay", "cs init failed");
    ESP_RETURN_ON_ERROR(BoardConfigureOutput(dc_), "GpioDisplay", "dc init failed");
    ESP_RETURN_ON_ERROR(BoardConfigureOutput(rst_, true), "GpioDisplay", "rst init failed");
    return BoardConfigureInput(busy_, false, false);
}
