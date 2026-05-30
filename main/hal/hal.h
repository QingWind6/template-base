#pragma once

#include <cstdint>

namespace HAL {

class Hal {
public:
    void init();

private:
    bool initialized_ = false;
};

Hal& GetHAL();

}  // namespace HAL
