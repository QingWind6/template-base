#pragma once

#include <cstdint>

enum class DisplayType : uint8_t {
    Unknown = 0,
    Epaper,
    Oled,
    Tft,
};

enum class DisplayColor : uint8_t {
    Mono = 0,
    Gray4,
    BWR,
    E6,
    Rgb565,
};

enum class DisplayPixelFormat : uint8_t {
    Mono1Msb = 0,
    Gray4,
    Gray8,
    Rgb565,
};

struct DisplayArea {
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t width = 0;
    uint16_t height = 0;
};

class Display {
public:
    virtual ~Display() = default;

    virtual DisplayType GetType() const;
    virtual uint16_t GetWidth() const = 0;
    virtual uint16_t GetHeight() const = 0;
    virtual DisplayColor GetColor() const = 0;

    virtual bool Probe();
    virtual bool Sleep() = 0;
    virtual bool Wakeup() = 0;
    virtual bool Clear(bool white = true) = 0;

    bool ClearWhite();
    bool ClearBlack();
};
