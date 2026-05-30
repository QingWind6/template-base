#pragma once

#include "power_managed.h"

#include <cstddef>
#include <cstdint>

struct AudioFormat {
    uint32_t sample_rate = 16000;
    uint8_t channels = 1;
    uint8_t bits_per_sample = 16;
    uint16_t frame_samples = 320;
};

class Microphone : public PowerManaged {
public:
    ~Microphone() override = default;

    virtual bool Start() = 0;
    virtual void Stop() = 0;
    virtual bool IsCapturing() const = 0;
    virtual AudioFormat GetFormat() const = 0;
    virtual std::size_t Read(int16_t* buffer, std::size_t samples, uint32_t timeout_ms) = 0;
};
