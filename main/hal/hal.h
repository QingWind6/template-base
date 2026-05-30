#pragma once

#include <cstdint>

namespace HAL {

class Hal {
public:
    void init();
};

Hal& GetHAL();

}  // namespace HAL
