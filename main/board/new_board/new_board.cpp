#include "config.h"

#include "board.h"

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
        ESP_LOGI(kTag, "Board is ready");
        if (led_ready_) {
            led_.Set(true);
        }
    }

    ~NewBoard() override {
        if (i2c_bus_) {
            i2c_del_master_bus(i2c_bus_);
        }
    }

    const char* GetName() const override {
        return HAL_BOARD_NAME;
    }

    Led* GetLed() override {
        if (!led_ready_) {
            return nullptr;
        }
        return &led_;
    }

    Button* GetButtonOk() override {
        if (!button_ok_ || !button_ok_->IsAvailable()) {
            return nullptr;
        }
        return button_ok_.get();
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
        const esp_err_t err = i2c_new_master_bus(&bus_config, &i2c_bus_);
        if (err != ESP_OK) {
            ESP_LOGW(kTag, "I2C bus init failed: %s", esp_err_to_name(err));
        }
    }

    void InitializeButtons() {
        button_ok_ = std::make_unique<Button>(static_cast<gpio_num_t>(HAL_BTN_OK));
        if (!button_ok_->IsAvailable()) {
            ESP_LOGW(kTag, "OK button is not available");
        }
    }

    void InitializeLed() {
        led_ready_ = led_.Init(static_cast<gpio_num_t>(HAL_LED_STATUS));
        if (!led_ready_) {
            ESP_LOGW(kTag, "status LED is not available");
        }
    }

    std::unique_ptr<Button> button_ok_;
    Led led_;
    i2c_master_bus_handle_t i2c_bus_ = nullptr;
    bool led_ready_ = false;
};

DECLARE_BOARD(NewBoard);
