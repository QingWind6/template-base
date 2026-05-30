#include "config.h"

#include "adc_battery.h"
#include "board.h"
#include "gpio_button.h"
#include "gpio_buzzer.h"
#include "gpio_display.h"
#include "gpio_led.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#include <memory>

namespace {
constexpr const char* kTag = "NewBoard";
}

class NewBoard : public Board {
public:
    NewBoard() {
        ESP_LOGI(kTag, "Initializing board: %s uuid=%s", GetBoardType(), GetUuid());
        InitializeButtons();
        InitializeLed();
        InitializeBuzzer();
        InitializeBattery();
        InitializeDisplay();
        InitializeSensors();
        ESP_LOGI(kTag, "Board is ready");
        if (_led_ready) {
            ESP_ERROR_CHECK(_led->Set(true));
        }
    }

    const char* GetName() const override {
        return HAL_BOARD_NAME;
    }

    Led* GetLed() override {
        if (!_led_ready || !_led) {
            return nullptr;
        }
        return _led.get();
    }

    Buzzer* GetBuzzer() override {
        if (!_buzzer_ready || !_buzzer) {
            return nullptr;
        }
        return _buzzer.get();
    }

    Battery* GetBattery() override {
        if (!_battery_ready || !_battery) {
            return nullptr;
        }
        return _battery.get();
    }

    Display* GetDisplay() override {
        if (!_display_ready || !_display) {
            return nullptr;
        }
        return _display.get();
    }

    EnvironmentSensor* GetEnvironmentSensor() override {
        if (!_environment_sensor_ready || !_environment_sensor) {
            return nullptr;
        }
        return _environment_sensor.get();
    }

    Imu* GetImu() override {
        if (!_imu_ready || !_imu) {
            return nullptr;
        }
        return _imu.get();
    }

    Rtc* GetRtc() override {
        if (!_rtc_ready || !_rtc) {
            return nullptr;
        }
        return _rtc.get();
    }

    Storage* GetStorage() override {
        if (!_storage_ready || !_storage) {
            return nullptr;
        }
        return _storage.get();
    }

    Microphone* GetMicrophone() override {
        if (!_microphone_ready || !_microphone) {
            return nullptr;
        }
        return _microphone.get();
    }

    Touch* GetTouch() override {
        if (!_touch_ready || !_touch) {
            return nullptr;
        }
        return _touch.get();
    }

    Button* GetButtonUp() override {
        if (!_button_up_ready || !_button_up) {
            return nullptr;
        }
        return _button_up.get();
    }

    Button* GetButtonDown() override {
        if (!_button_down_ready || !_button_down) {
            return nullptr;
        }
        return _button_down.get();
    }

    Button* GetButtonOk() override {
        if (!_button_ok_ready || !_button_ok) {
            return nullptr;
        }
        return _button_ok.get();
    }

    Button* GetButton() override {
        return GetButtonOk();
    }

private:
    void InitializeButtons() {
        if (HAL_BTN_UP != GPIO_NUM_NC) {
            _button_up = std::make_unique<GpioButton>(HAL_BTN_UP);
        }
        if (HAL_BTN_DOWN != GPIO_NUM_NC) {
            _button_down = std::make_unique<GpioButton>(HAL_BTN_DOWN);
        }
        if (HAL_BTN_OK != GPIO_NUM_NC) {
            _button_ok = std::make_unique<GpioButton>(HAL_BTN_OK);
        }

        _button_up_ready = InitializeButton(_button_up, "button up");
        _button_down_ready = InitializeButton(_button_down, "button down");
        _button_ok_ready = InitializeButton(_button_ok, "button ok");
    }

    void InitializeLed() {
        if (HAL_LED_STATUS == GPIO_NUM_NC) {
            ESP_LOGI(kTag, "status led not configured");
            return;
        }

        _led = std::make_unique<GpioLed>(HAL_LED_STATUS);
        const esp_err_t err = _led->Initialize();
        if (err != ESP_OK) {
            ESP_LOGW(kTag, "status led init failed: %s", esp_err_to_name(err));
            _led.reset();
            return;
        }

        _led_ready = true;
        ESP_LOGI(kTag, "status led ready");
    }

    void InitializeBuzzer() {
        if (HAL_BUZZER == GPIO_NUM_NC) {
            ESP_LOGI(kTag, "buzzer not configured");
            return;
        }

        _buzzer = std::make_unique<GpioBuzzer>(HAL_BUZZER);
        const esp_err_t err = _buzzer->Initialize();
        if (err != ESP_OK) {
            ESP_LOGW(kTag, "buzzer init failed: %s", esp_err_to_name(err));
            _buzzer.reset();
            return;
        }

        _buzzer_ready = true;
        ESP_LOGI(kTag, "buzzer ready");
    }

    void InitializeBattery() {
        _battery = std::make_unique<AdcBattery>(HAL_BATTERY_ADC_UNIT, HAL_BATTERY_ADC_CHANNEL, HAL_BATTERY_CHARGE_DETECT);
        const esp_err_t err = _battery->Initialize();
        if (err != ESP_OK) {
            ESP_LOGW(kTag, "battery init failed: %s", esp_err_to_name(err));
            _battery.reset();
            return;
        }

        _battery_ready = true;
        ESP_LOGI(kTag, "battery ready");
    }

    void InitializeDisplay() {
        const bool has_display = HAL_EPD_SCK != GPIO_NUM_NC && HAL_EPD_MOSI != GPIO_NUM_NC &&
                                 HAL_EPD_CS != GPIO_NUM_NC && HAL_EPD_DC != GPIO_NUM_NC &&
                                 HAL_EPD_RST != GPIO_NUM_NC && HAL_EPD_BUSY != GPIO_NUM_NC;
        if (!has_display) {
            ESP_LOGI(kTag, "display not configured");
            return;
        }

        _display = std::make_unique<GpioDisplay>(HAL_EPD_SCK,
                                                 HAL_EPD_MOSI,
                                                 HAL_EPD_CS,
                                                 HAL_EPD_DC,
                                                 HAL_EPD_RST,
                                                 HAL_EPD_BUSY);
        const esp_err_t err = _display->Initialize();
        if (err != ESP_OK) {
            ESP_LOGW(kTag, "display init failed: %s", esp_err_to_name(err));
            _display.reset();
            return;
        }

        _display_ready = true;
        ESP_LOGI(kTag, "display ready");
    }

    void InitializeSensors() {
        // Board-specific chip implementations belong in board/new_board/devices.
        _environment_sensor_ready = _environment_sensor != nullptr;
        _imu_ready = _imu != nullptr;
        _rtc_ready = _rtc != nullptr;
        _storage_ready = _storage != nullptr;
        _microphone_ready = _microphone != nullptr;
        _touch_ready = _touch != nullptr;
    }

    bool InitializeButton(std::unique_ptr<Button>& button, const char* name) {
        if (!button) {
            ESP_LOGI(kTag, "%s not configured", name);
            return false;
        }

        const esp_err_t err = button->Initialize();
        if (err != ESP_OK) {
            ESP_LOGW(kTag, "%s init failed: %s", name, esp_err_to_name(err));
            button.reset();
            return false;
        }

        ESP_LOGI(kTag, "%s ready", name);
        return true;
    }

    std::unique_ptr<Button> _button_up;
    std::unique_ptr<Button> _button_down;
    std::unique_ptr<Button> _button_ok;
    std::unique_ptr<Led> _led;
    std::unique_ptr<Buzzer> _buzzer;
    std::unique_ptr<Battery> _battery;
    std::unique_ptr<Display> _display;
    std::unique_ptr<EnvironmentSensor> _environment_sensor;
    std::unique_ptr<Imu> _imu;
    std::unique_ptr<Rtc> _rtc;
    std::unique_ptr<Storage> _storage;
    std::unique_ptr<Microphone> _microphone;
    std::unique_ptr<Touch> _touch;

    bool _button_up_ready = false;
    bool _button_down_ready = false;
    bool _button_ok_ready = false;
    bool _led_ready = false;
    bool _buzzer_ready = false;
    bool _battery_ready = false;
    bool _display_ready = false;
    bool _environment_sensor_ready = false;
    bool _imu_ready = false;
    bool _rtc_ready = false;
    bool _storage_ready = false;
    bool _microphone_ready = false;
    bool _touch_ready = false;
};

DECLARE_BOARD(NewBoard);
