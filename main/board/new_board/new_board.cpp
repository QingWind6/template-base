#include "config.h"

#include "adc_battery.h"
#include "board.h"
#include "gpio_button.h"
#include "gpio_display.h"
#include "gpio_led.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"
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
        InitializeI2c();
        InitializeButtons();
        InitializeLed();
        InitializeBattery();
        InitializeDisplay();
        ESP_LOGI(kTag, "Board is ready");
        if (_led_ready) {
            ESP_ERROR_CHECK(_led->Set(true));
        }
    }

    ~NewBoard() override {
        if (_i2c_bus) {
            i2c_del_master_bus(_i2c_bus);
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
    void InitializeI2c() {
        i2c_master_bus_config_t bus_config = {};
        bus_config.i2c_port = I2C_NUM_0;
        bus_config.scl_io_num = static_cast<gpio_num_t>(HAL_I2C_SCL);
        bus_config.sda_io_num = static_cast<gpio_num_t>(HAL_I2C_SDA);
        bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
        bus_config.glitch_ignore_cnt = 7;
        bus_config.flags.enable_internal_pullup = true;
        _i2c_ready = i2c_new_master_bus(&bus_config, &_i2c_bus) == ESP_OK;
    }

    void InitializeButtons() {
        _button_ok = std::make_unique<GpioButton>(static_cast<gpio_num_t>(HAL_BTN_OK));
        _button_ok_ready = _button_ok->Initialize() == ESP_OK;
    }

    void InitializeLed() {
        _led = std::make_unique<GpioLed>(static_cast<gpio_num_t>(HAL_LED_STATUS));
        _led_ready = _led->Initialize() == ESP_OK;
    }

    void InitializeBattery() {
        _battery = std::make_unique<AdcBattery>(HAL_BATTERY_ADC_UNIT,
                                                HAL_BATTERY_ADC_CHANNEL,
                                                static_cast<gpio_num_t>(HAL_BATTERY_CHARGE_DETECT));
        _battery_ready = _battery->Initialize() == ESP_OK;
    }

    void InitializeDisplay() {
        _display = std::make_unique<GpioDisplay>(static_cast<gpio_num_t>(HAL_EPD_SCK),
                                                 static_cast<gpio_num_t>(HAL_EPD_MOSI),
                                                 static_cast<gpio_num_t>(HAL_EPD_CS),
                                                 static_cast<gpio_num_t>(HAL_EPD_DC),
                                                 static_cast<gpio_num_t>(HAL_EPD_RST),
                                                 static_cast<gpio_num_t>(HAL_EPD_BUSY));
        _display_ready = _display->Initialize() == ESP_OK;
    }

    std::unique_ptr<Button> _button_ok;
    std::unique_ptr<Led> _led;
    std::unique_ptr<Battery> _battery;
    std::unique_ptr<Display> _display;
    i2c_master_bus_handle_t _i2c_bus = nullptr;

    bool _i2c_ready = false;
    bool _button_ok_ready = false;
    bool _led_ready = false;
    bool _battery_ready = false;
    bool _display_ready = false;
};

DECLARE_BOARD(NewBoard);
