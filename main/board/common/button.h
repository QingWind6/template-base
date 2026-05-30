#ifndef BUTTON_H_
#define BUTTON_H_

#include <driver/gpio.h>
#include <iot_button.h>
#include <button_types.h>
#include <button_adc.h>
#include <button_gpio.h>
#include <functional>
#include <memory>
#include <vector>

class Button {
public:
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = delete;
    Button& operator=(Button&&) = delete;

    explicit Button(button_handle_t button_handle);
    Button(gpio_num_t gpio_num, bool active_high = false, uint16_t long_press_time = 0, uint16_t short_press_time = 0, bool enable_power_save = false);
    ~Button();

    bool IsAvailable() const { return button_handle_ != nullptr; }
    bool IsPressed() const;
    gpio_num_t GetGpioNum() const { return gpio_num_; }
    button_handle_t GetHandle() const { return button_handle_; }

    void OnPressDown(std::function<void()> callback);
    void OnPressUp(std::function<void()> callback);
    void OnLongPress(std::function<void()> callback);
    void OnLongPress(std::function<void()> callback, uint16_t press_time_ms);
    void OnRepeat(std::function<void()> callback);
    void OnClick(std::function<void()> callback);
    void OnDoubleClick(std::function<void()> callback);
    void OnMultipleClick(std::function<void()> callback, uint8_t click_count = 3);

protected:
    struct CallbackRegistration {
        std::function<void()> callback;
        button_event_args_t event_args = {};
        bool has_event_args = false;
    };

    void RegisterCallback(button_event_t event, std::function<void()> callback, const button_event_args_t* event_args = nullptr);
    static void EventDispatcher(void* handle, void* usr_data);

    gpio_num_t gpio_num_ = GPIO_NUM_NC;
    button_handle_t button_handle_ = nullptr;
    std::vector<std::unique_ptr<CallbackRegistration>> callback_registrations_;
};

#if CONFIG_SOC_ADC_SUPPORTED
class AdcButton : public Button {
public:
    AdcButton(const button_adc_config_t& adc_config);
};
#endif

class PowerSaveButton : public Button {
public:
    PowerSaveButton(gpio_num_t gpio_num) : Button(gpio_num, false, 0, 0, true) {
    }
};

#endif // BUTTON_H_
