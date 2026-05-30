#pragma once

#include <cstddef>
#include <cstdint>

#include "low_power_controllable.h"

struct AudioFormat_t {
    uint32_t sample_rate = 16000;
    uint8_t channels = 1;
    uint8_t bits_per_sample = 16;
    uint16_t frame_samples = 320;
};

class Microphone : public LowPowerControllable {
public:
    virtual ~Microphone() = default;

    virtual void init() {}
    virtual bool isAvailable() { return false; }
    virtual bool start() { return false; }
    virtual void stop() {}
    virtual bool isCapturing() { return false; }

    virtual AudioFormat_t getFormat() { return {}; }

    virtual size_t read(int16_t* buffer, size_t samples, uint32_t timeout_ms)
    {
        (void)buffer;
        (void)samples;
        (void)timeout_ms;
        return 0;
    }
};
