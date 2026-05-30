#include "hal.h"

#include "board.h"

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <algorithm>
#include <array>
#include <ctime>
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
    ESP_LOGI(TAG, "Initializing HAL");
    // Board& board = Board::GetBoard();
}

}  // namespace HAL
