#pragma once

#include <cstdint>

namespace HAL {

class Hal {
public:
    void init();

private:

};

Hal& GetHAL();

}  // namespace HAL
