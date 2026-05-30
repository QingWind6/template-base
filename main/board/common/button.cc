#include "button.h"

#include "gpio_isr_service_hint.h"

#include <button_gpio.h>
#include <esp_err.h>
#include <esp_log.h>

#define TAG "Button"

#if CONFIG_SOC_ADC_SUPPORTED
AdcButton::AdcButton(const button_adc_config_t& adc_config) : Button(nullptr) {
    button_config_t btn_config = {
        .long_press_time = 2000,
        .short_press_time = 0,
    };
    ESP_ERROR_CHECK(iot_button_new_adc_device(&btn_config, &adc_config, &button_handle_));
}
#endif

Button::Button(button_handle_t button_handle) : button_handle_(button_handle) {
}

Button::Button(gpio_num_t gpio_num, bool active_high, uint16_t long_press_time, uint16_t short_press_time, bool enable_power_save) : gpio_num_(gpio_num) {
    if (gpio_num == GPIO_NUM_NC) {
        return;
    }
    button_config_t button_config = {
        .long_press_time = long_press_time,
        .short_press_time = short_press_time
    };
    button_gpio_config_t gpio_config = {
        .gpio_num = gpio_num,
        .active_level = static_cast<uint8_t>(active_high ? 1 : 0),
        .enable_power_save = enable_power_save,
        .disable_pull = false
    };
    ESP_ERROR_CHECK(iot_button_new_gpio_device(&button_config, &gpio_config, &button_handle_));
    if (button_handle_ != nullptr && enable_power_save) {
        Common::SetGpioIsrServiceInstalledHint(true);
    }
}

Button::~Button() {
    callback_registrations_.clear();
    if (button_handle_ != NULL) {
        iot_button_delete(button_handle_);
    }
}

bool Button::IsPressed() const {
    if (button_handle_ == nullptr) {
        return false;
    }
    return iot_button_get_key_level(button_handle_) == 1;
}

void Button::OnPressDown(std::function<void()> callback) {
    RegisterCallback(BUTTON_PRESS_DOWN, std::move(callback));
}

void Button::OnPressUp(std::function<void()> callback) {
    RegisterCallback(BUTTON_PRESS_UP, std::move(callback));
}

void Button::OnLongPress(std::function<void()> callback) {
    RegisterCallback(BUTTON_LONG_PRESS_START, std::move(callback));
}

void Button::OnLongPress(std::function<void()> callback, uint16_t press_time_ms) {
    button_event_args_t event_args = {
        .long_press = {
            .press_time = press_time_ms
        }
    };
    RegisterCallback(BUTTON_LONG_PRESS_START, std::move(callback), &event_args);
}

void Button::OnRepeat(std::function<void()> callback) {
    RegisterCallback(BUTTON_PRESS_REPEAT, std::move(callback));
}

void Button::OnClick(std::function<void()> callback) {
    RegisterCallback(BUTTON_SINGLE_CLICK, std::move(callback));
}

void Button::OnDoubleClick(std::function<void()> callback) {
    RegisterCallback(BUTTON_DOUBLE_CLICK, std::move(callback));
}

void Button::OnMultipleClick(std::function<void()> callback, uint8_t click_count) {
    button_event_args_t event_args = {
        .multiple_clicks = {
            .clicks = click_count
        }
    };
    RegisterCallback(BUTTON_MULTIPLE_CLICK, std::move(callback), &event_args);
}

void Button::RegisterCallback(button_event_t event, std::function<void()> callback, const button_event_args_t* event_args)
{
    if(button_handle_ == nullptr || !callback)
    {
        return;
    }

    auto registration = std::make_unique<CallbackRegistration>();
    registration->callback = std::move(callback);
    registration->has_event_args = (event_args != nullptr);
    if(event_args != nullptr)
    {
        registration->event_args = *event_args;
    }

    button_event_args_t* args_ptr = registration->has_event_args ? &registration->event_args : nullptr;
    const esp_err_t err = iot_button_register_cb(button_handle_, event, args_ptr, EventDispatcher, registration.get());
    if(err != ESP_OK)
    {
        ESP_LOGW(TAG,
                 "register button callback failed event=%d: %s",
                 static_cast<int>(event),
                 esp_err_to_name(err));
        return;
    }

    callback_registrations_.push_back(std::move(registration));
}

void Button::EventDispatcher(void* handle, void* usr_data)
{
    (void)handle;

    auto* registration = static_cast<CallbackRegistration*>(usr_data);
    if(registration != nullptr && registration->callback)
    {
        registration->callback();
    }
}
