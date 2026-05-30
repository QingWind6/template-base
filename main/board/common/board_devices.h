#pragma once

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#include <memory>
#include <utility>

inline bool BoardHasGpio(gpio_num_t pin) {
    return pin != GPIO_NUM_NC;
}

template <typename Device, typename... Args>
std::unique_ptr<Device> MakeDeviceIf(bool enabled, Args&&... args) {
    if (!enabled) {
        return nullptr;
    }
    return std::make_unique<Device>(std::forward<Args>(args)...);
}

template <typename Device>
bool InitializeOwnedDevice(std::unique_ptr<Device>& device, const char* tag, const char* name) {
    if (!device) {
        ESP_LOGI(tag, "%s not configured", name);
        return false;
    }

    const esp_err_t err = device->Initialize();
    if (err != ESP_OK) {
        ESP_LOGW(tag, "%s init failed: %s", name, esp_err_to_name(err));
        device.reset();
        return false;
    }

    ESP_LOGI(tag, "%s ready", name);
    return true;
}

template <typename Device>
Device* GetReadyDevice(const std::unique_ptr<Device>& device, bool ready) {
    if (!ready || !device) {
        return nullptr;
    }
    return device.get();
}
