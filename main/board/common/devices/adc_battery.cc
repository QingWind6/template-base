#include "adc_battery.h"

#include "board_gpio.h"

#include "esp_check.h"

AdcBattery::AdcBattery(adc_unit_t unit, adc_channel_t channel, gpio_num_t charge_detect_pin)
    : unit_(unit), channel_(channel), charge_detect_pin_(charge_detect_pin) {}

AdcBattery::~AdcBattery() {
    if (adc_handle_ != nullptr) {
        adc_oneshot_del_unit(adc_handle_);
    }
}

esp_err_t AdcBattery::Initialize() {
    adc_oneshot_unit_init_cfg_t unit_config = {};
    unit_config.unit_id = unit_;
    ESP_RETURN_ON_ERROR(adc_oneshot_new_unit(&unit_config, &adc_handle_), "AdcBattery", "adc unit init failed");

    adc_oneshot_chan_cfg_t channel_config = {};
    channel_config.bitwidth = ADC_BITWIDTH_DEFAULT;
    channel_config.atten = ADC_ATTEN_DB_12;
    ESP_RETURN_ON_ERROR(adc_oneshot_config_channel(adc_handle_, channel_, &channel_config), "AdcBattery", "adc channel config failed");

    return BoardConfigureInput(charge_detect_pin_, true, false);
}

esp_err_t AdcBattery::ReadRaw(int* raw) {
    if (adc_handle_ == nullptr || raw == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }
    return adc_oneshot_read(adc_handle_, channel_, raw);
}

bool AdcBattery::IsCharging() const {
    return BoardGetGpioLevel(charge_detect_pin_);
}
