#pragma once

#include "battery_gauge.h"
#include "button.h"
#include "buzzer.h"
#include "display.h"
#include "epaper_display.h"
#include "imu_device.h"
#include "led.h"
#include "microphone.h"
#include "sd_card.h"
#include "touch.h"
#include "ui_orientation.h"

#include <string>

class Board {
protected:
    Board();

public:
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    virtual ~Board() = default;

    static Board& GetInstance();

    virtual const char* GetName() const = 0;
    virtual const char* GetBoardType() const;
    virtual const char* GetScreenType() const;
    virtual const char* GetScreenResolution() const;
    const char* GetUuid() const;

    virtual void PowerOn();
    virtual void PowerOff();
    virtual void Reboot();
    virtual void PrepareForDeepSleep();

    virtual Led* GetLed() {
        return nullptr;
    }

    virtual Buzzer* GetBuzzer() {
        return nullptr;
    }

    virtual BatteryGauge* GetBatteryGauge() {
        return nullptr;
    }

    virtual BatteryGauge* GetBattery() {
        return GetBatteryGauge();
    }

    virtual EpaperDisplay* GetEpaperDisplay() {
        return nullptr;
    }

    virtual Display* GetDisplay() {
        return GetEpaperDisplay();
    }

    virtual SdCard* GetSdCard() {
        return nullptr;
    }

    virtual SdCard* GetStorage() {
        return GetSdCard();
    }

    virtual ImuDevice* GetImu() {
        return nullptr;
    }

    virtual Microphone* GetMicrophone() {
        return nullptr;
    }

    virtual TouchDevice* GetTouch() {
        return nullptr;
    }

    virtual const Ui::Config& GetUiConfig() const;

    virtual Button* GetButton() {
        return GetButtonOk();
    }

    virtual Button* GetButtonUp() {
        return nullptr;
    }

    virtual Button* GetButtonDown() {
        return nullptr;
    }

    virtual Button* GetButtonOk() {
        return nullptr;
    }

protected:
    std::string GenerateUuid() const;

private:
    std::string uuid_;
};

#define DECLARE_BOARD(BOARD_CLASS)                  \
    Board& Board::GetInstance() {                   \
        static BOARD_CLASS board_instance;          \
        return board_instance;                      \
    }
