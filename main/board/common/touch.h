#pragma once

#include <cstdint>

#include "activity_callback.h"

struct TouchPoint_t {
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t size = 0;
    uint8_t id = 0;
};

struct TouchData_t {
    static constexpr uint8_t kMaxPoints = 5;

    bool valid = false;
    uint8_t count = 0;
    uint32_t timestamp_ms = 0;
    TouchPoint_t points[kMaxPoints] = {};
};

class TouchDevice {
public:
    virtual ~TouchDevice() = default;

    virtual void init() {}
    virtual void update() {}
    virtual bool isAvailable() { return false; }
    virtual void SetActivityCallback(Common::ActivityCallback callback, void* user_data)
    {
        (void)callback;
        (void)user_data;
    }
    virtual bool ConfigureLightSleepWakeup(bool enabled)
    {
        (void)enabled;
        return false;
    }
    virtual bool IsLightSleepWakeupEnabled() const
    {
        return false;
    }

    virtual bool getData(TouchData_t& touch_data)
    {
        (void)touch_data;
        return false;
    }
};
