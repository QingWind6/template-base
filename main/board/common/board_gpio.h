#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

inline esp_err_t BoardConfigureOutput(gpio_num_t pin, bool initial_level = false) {
    if (pin == GPIO_NUM_NC) {
        return ESP_OK;
    }

    gpio_config_t config = {};
    config.pin_bit_mask = 1ULL << pin;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_up_en = GPIO_PULLUP_DISABLE;
    config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;

    esp_err_t err = gpio_config(&config);
    if (err != ESP_OK) {
        return err;
    }
    return gpio_set_level(pin, initial_level ? 1 : 0);
}

inline esp_err_t BoardConfigureInput(gpio_num_t pin, bool pull_up = true, bool pull_down = false) {
    if (pin == GPIO_NUM_NC) {
        return ESP_OK;
    }

    gpio_config_t config = {};
    config.pin_bit_mask = 1ULL << pin;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = pull_up ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
    config.pull_down_en = pull_down ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
    config.intr_type = GPIO_INTR_DISABLE;
    return gpio_config(&config);
}

inline esp_err_t BoardSetGpioLevel(gpio_num_t pin, bool level) {
    if (pin == GPIO_NUM_NC) {
        return ESP_OK;
    }
    return gpio_set_level(pin, level ? 1 : 0);
}

inline bool BoardGetGpioLevel(gpio_num_t pin) {
    if (pin == GPIO_NUM_NC) {
        return false;
    }
    return gpio_get_level(pin) != 0;
}
