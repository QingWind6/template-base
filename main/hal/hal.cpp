#include "hal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include <memory>

namespace HAL {

static std::unique_ptr<Hal> hal_instance;
static constexpr const char* TAG = "HAL";


Hal& GetHAL()
{
    if (hal_instance == nullptr)
    {
        hal_instance = std::make_unique<Hal>();
    }
    return *hal_instance;
}

void Hal::init() {

}

}  // namespace HAL
