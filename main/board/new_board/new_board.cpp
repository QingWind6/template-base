#include "config.h"

#include "adc_battery.h"
#include "board.h"
#include "board_devices.h"
#include "gpio_button.h"
#include "gpio_buzzer.h"
#include "gpio_display.h"
#include "gpio_led.h"

#include "esp_err.h"
#include "esp_log.h"

#include <memory>

namespace {
constexpr const char* kTag = "NewBoard";
}

class NewBoard : public Board {
public:
    const char* GetName() const override {
        return HAL_BOARD_NAME;
    }

    void Initialize() override {
        Board::Initialize();

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

    Led* GetLed() override {
        return GetReadyDevice(_led, _led_ready);
    }

    Buzzer* GetBuzzer() override {
        return GetReadyDevice(_buzzer, _buzzer_ready);
    }

    Battery* GetBattery() override {
        return GetReadyDevice(_battery, _battery_ready);
    }

    Display* GetDisplay() override {
        return GetReadyDevice(_display, _display_ready);
    }

    EnvironmentSensor* GetEnvironmentSensor() override {
        return GetReadyDevice(_environment_sensor, _environment_sensor_ready);
    }

    Imu* GetImu() override {
        return GetReadyDevice(_imu, _imu_ready);
    }

    Rtc* GetRtc() override {
        return GetReadyDevice(_rtc, _rtc_ready);
    }

    Storage* GetStorage() override {
        return GetReadyDevice(_storage, _storage_ready);
    }

    Microphone* GetMicrophone() override {
        return GetReadyDevice(_microphone, _microphone_ready);
    }

    Touch* GetTouch() override {
        return GetReadyDevice(_touch, _touch_ready);
    }

    Button* GetButtonUp() override {
        return GetReadyDevice(_button_up, _button_up_ready);
    }

    Button* GetButtonDown() override {
        return GetReadyDevice(_button_down, _button_down_ready);
    }

    Button* GetButtonOk() override {
        return GetReadyDevice(_button_ok, _button_ok_ready);
    }

    Button* GetButton() override {
        return GetButtonOk();
    }

private:
    void InitializeButtons() {
        if (!_button_up) {
            _button_up = MakeDeviceIf<GpioButton>(BoardHasGpio(HAL_BTN_UP), HAL_BTN_UP);
        }
        if (!_button_down) {
            _button_down = MakeDeviceIf<GpioButton>(BoardHasGpio(HAL_BTN_DOWN), HAL_BTN_DOWN);
        }
        if (!_button_ok) {
            _button_ok = MakeDeviceIf<GpioButton>(BoardHasGpio(HAL_BTN_OK), HAL_BTN_OK);
        }

        _button_up_ready = InitializeOwnedDevice(_button_up, kTag, "button up");
        _button_down_ready = InitializeOwnedDevice(_button_down, kTag, "button down");
        _button_ok_ready = InitializeOwnedDevice(_button_ok, kTag, "button ok");
    }

    void InitializeLed() {
        if (!_led) {
            _led = MakeDeviceIf<GpioLed>(BoardHasGpio(HAL_LED_STATUS), HAL_LED_STATUS);
        }
        _led_ready = InitializeOwnedDevice(_led, kTag, "status led");
    }

    void InitializeBuzzer() {
        if (!_buzzer) {
            _buzzer = MakeDeviceIf<GpioBuzzer>(BoardHasGpio(HAL_BUZZER), HAL_BUZZER);
        }
        _buzzer_ready = InitializeOwnedDevice(_buzzer, kTag, "buzzer");
    }

    void InitializeBattery() {
        if (!_battery) {
            _battery = std::make_unique<AdcBattery>(HAL_BATTERY_ADC_UNIT, HAL_BATTERY_ADC_CHANNEL, HAL_BATTERY_CHARGE_DETECT);
        }
        _battery_ready = InitializeOwnedDevice(_battery, kTag, "battery");
    }

    void InitializeDisplay() {
        const bool has_display = BoardHasGpio(HAL_EPD_SCK) && BoardHasGpio(HAL_EPD_MOSI) &&
                                 BoardHasGpio(HAL_EPD_CS) && BoardHasGpio(HAL_EPD_DC) &&
                                 BoardHasGpio(HAL_EPD_RST) && BoardHasGpio(HAL_EPD_BUSY);
        if (!_display) {
            _display = MakeDeviceIf<GpioDisplay>(has_display, HAL_EPD_SCK, HAL_EPD_MOSI, HAL_EPD_CS, HAL_EPD_DC, HAL_EPD_RST, HAL_EPD_BUSY);
        }
        _display_ready = InitializeOwnedDevice(_display, kTag, "display");
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
