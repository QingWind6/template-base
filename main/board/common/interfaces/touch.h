#pragma once

#include "power_managed.h"

#include <cstddef>
#include <cstdint>

struct TouchPoint {
    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t id = 0;
    uint8_t pressure = 0;
};

class Touch : public PowerManaged {
public:
    ~Touch() override = default;

    virtual bool Read(TouchPoint* points, std::size_t max_points, std::size_t* point_count) = 0;
};
