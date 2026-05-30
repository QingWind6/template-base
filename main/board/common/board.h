#pragma once

#include "battery.h"
#include "button.h"
#include "buzzer.h"
#include "display.h"
#include "environment_sensor.h"
#include "imu.h"
#include "led.h"
#include "microphone.h"
#include "rtc.h"
#include "storage.h"
#include "touch.h"

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

    virtual void Initialize();
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

    virtual Battery* GetBattery() {
        return nullptr;
    }

    virtual Display* GetDisplay() {
        return nullptr;
    }

    virtual Rtc* GetRtc() {
        return nullptr;
    }

    virtual Storage* GetStorage() {
        return nullptr;
    }

    virtual EnvironmentSensor* GetEnvironmentSensor() {
        return nullptr;
    }

    virtual Imu* GetImu() {
        return nullptr;
    }

    virtual Microphone* GetMicrophone() {
        return nullptr;
    }

    virtual Touch* GetTouch() {
        return nullptr;
    }

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
