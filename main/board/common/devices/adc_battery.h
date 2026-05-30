#pragma once

#include "battery.h"

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

class AdcBattery : public Battery {
public:
    AdcBattery(adc_unit_t unit, adc_channel_t channel, gpio_num_t charge_detect_pin = GPIO_NUM_NC);
    ~AdcBattery() override;

    esp_err_t Initialize() override;
    esp_err_t ReadRaw(int* raw) override;
    bool IsCharging() const override;

private:
    adc_unit_t unit_;
    adc_channel_t channel_;
    gpio_num_t charge_detect_pin_;
    adc_oneshot_unit_handle_t adc_handle_ = nullptr;
};
