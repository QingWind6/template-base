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
static constexpr const char* kStorageMountPoint = "/sdcard";
static constexpr TickType_t kShutdownPollTicks = pdMS_TO_TICKS(50);
static constexpr TickType_t kOkLongPressTicks = pdMS_TO_TICKS(2500);
static TaskHandle_t shutdown_task_handle = nullptr;

namespace {

void ProbeButtons(Board& board)
{
    struct ButtonProbe {
        const char* name;
        Button* button;
    };

    const ButtonProbe buttons[] = {
        {"up", board.GetButtonUp()},
        {"down", board.GetButtonDown()},
        {"ok", board.GetButtonOk()},
    };

    for (const auto& item : buttons) {
        if (item.button == nullptr) {
            ESP_LOGI(TAG, "button %s: unavailable", item.name);
            continue;
        }
        ESP_LOGI(TAG, "button %s: %s", item.name, item.button->IsPressed() ? "pressed" : "released");
    }
}

void ProbeBuzzer(Board& board)
{
    auto* buzzer = board.GetBuzzer();
    if (buzzer == nullptr) {
        ESP_LOGI(TAG, "buzzer: unavailable");
        return;
    }

    ESP_LOGI(TAG, "buzzer: pulse");
    if (buzzer->Set(true) == ESP_OK) {
        vTaskDelay(pdMS_TO_TICKS(80));
        buzzer->Set(false);
    }
}

void ProbeBattery(Board& board)
{
    auto* battery = board.GetBattery();
    if (battery == nullptr) {
        ESP_LOGI(TAG, "battery: unavailable");
        return;
    }

    int raw = 0;
    const esp_err_t err = battery->ReadRaw(&raw);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "battery: raw=%d charging=%s", raw, battery->IsCharging() ? "yes" : "no");
    } else {
        ESP_LOGW(TAG, "battery: read failed: %s", esp_err_to_name(err));
    }
}

void ProbeEnvironment(Board& board)
{
    auto* sensor = board.GetEnvironmentSensor();
    if (sensor == nullptr) {
        ESP_LOGI(TAG, "environment: unavailable");
        return;
    }

    EnvironmentReading reading = {};
    if (!sensor->Read(&reading)) {
        ESP_LOGW(TAG, "environment: read failed");
        return;
    }

    ESP_LOGI(TAG,
             "environment: temp=%s%.2fC humidity=%s%.2f%% pressure=%s%.2fhPa",
             reading.has_temperature ? "" : "n/a ",
             reading.temperature_c,
             reading.has_humidity ? "" : "n/a ",
             reading.humidity_percent,
             reading.has_pressure ? "" : "n/a ",
             reading.pressure_hpa);
}

void ProbeImu(Board& board)
{
    auto* imu = board.GetImu();
    if (imu == nullptr) {
        ESP_LOGI(TAG, "imu: unavailable");
        return;
    }

    ImuReading reading = {};
    if (!imu->Read(&reading)) {
        ESP_LOGW(TAG, "imu: read failed");
        return;
    }

    ESP_LOGI(TAG,
             "imu: accel=(%.3f, %.3f, %.3f) gyro=(%.3f, %.3f, %.3f) temp=%s%.2fC",
             reading.accel_x,
             reading.accel_y,
             reading.accel_z,
             reading.gyro_x,
             reading.gyro_y,
             reading.gyro_z,
             reading.has_temperature ? "" : "n/a ",
             reading.temperature_c);
}

void ProbeRtc(Board& board)
{
    auto* rtc = board.GetRtc();
    if (rtc == nullptr) {
        ESP_LOGI(TAG, "rtc: unavailable");
        return;
    }

    std::tm timeinfo = {};
    if (!rtc->GetTime(&timeinfo)) {
        ESP_LOGW(TAG, "rtc: read failed");
        return;
    }

    ESP_LOGI(TAG,
             "rtc: %04d-%02d-%02d %02d:%02d:%02d",
             timeinfo.tm_year + 1900,
             timeinfo.tm_mon + 1,
             timeinfo.tm_mday,
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);
}

void ProbeTouch(Board& board)
{
    auto* touch = board.GetTouch();
    if (touch == nullptr) {
        ESP_LOGI(TAG, "touch: unavailable");
        return;
    }

    std::array<TouchPoint, 5> points = {};
    std::size_t point_count = 0;
    if (!touch->Read(points.data(), points.size(), &point_count)) {
        ESP_LOGW(TAG, "touch: read failed");
        return;
    }

    ESP_LOGI(TAG, "touch: %u point(s)", static_cast<unsigned>(point_count));
    for (std::size_t i = 0; i < point_count; ++i) {
        ESP_LOGI(TAG,
                 "touch[%u]: id=%u x=%u y=%u pressure=%u",
                 static_cast<unsigned>(i),
                 points[i].id,
                 points[i].x,
                 points[i].y,
                 points[i].pressure);
    }
}

void ProbeMicrophone(Board& board)
{
    auto* microphone = board.GetMicrophone();
    if (microphone == nullptr) {
        ESP_LOGI(TAG, "microphone: unavailable");
        return;
    }

    const AudioFormat format = microphone->GetFormat();
    std::array<int16_t, 160> samples = {};
    const std::size_t requested = std::min<std::size_t>(samples.size(), format.frame_samples);

    if (!microphone->Start()) {
        ESP_LOGW(TAG, "microphone: start failed");
        return;
    }

    const std::size_t read = microphone->Read(samples.data(), requested, 200);
    microphone->Stop();

    ESP_LOGI(TAG,
             "microphone: format=%luHz/%ubit/%uch samples=%u",
             static_cast<unsigned long>(format.sample_rate),
             format.bits_per_sample,
             format.channels,
             static_cast<unsigned>(read));
}

void ProbeStorage(Board& board)
{
    auto* storage = board.GetStorage();
    if (storage == nullptr) {
        ESP_LOGI(TAG, "storage: unavailable");
        return;
    }

    const bool inserted = storage->IsInserted();
    ESP_LOGI(TAG, "storage: inserted=%s mounted=%s", inserted ? "yes" : "no", storage->IsMounted() ? "yes" : "no");
    if (!inserted || storage->IsMounted()) {
        return;
    }

    if (storage->Mount(kStorageMountPoint)) {
        ESP_LOGI(TAG, "storage: mounted at %s", storage->GetBasePath());
    } else {
        ESP_LOGW(TAG, "storage: mount failed");
    }
}

void ProbeDisplay(Board& board)
{
    auto* display = board.GetDisplay();
    ESP_LOGI(TAG,
             "display: %s type=%s resolution=%s",
             display != nullptr ? "ready" : "unavailable",
             board.GetScreenType(),
             board.GetScreenResolution());
    if (display == nullptr) {
        return;
    }

    const esp_err_t err = display->Clear(true);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "display: full clear refresh requested");
    } else if (err != ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "display: clear failed: %s", esp_err_to_name(err));
    }
}

void ProbeLed(Board& board)
{
    auto* led = board.GetLed();
    if (led == nullptr) {
        ESP_LOGI(TAG, "led: unavailable");
        return;
    }

    ESP_LOGI(TAG, "led: toggle");
    if (led->Toggle() == ESP_OK) {
        vTaskDelay(pdMS_TO_TICKS(80));
        led->Toggle();
    }
}

void ProbeBoardDevices(Board& board)
{
    ESP_LOGI(TAG, "Board probe start");
    ProbeLed(board);
    ProbeButtons(board);
    ProbeBuzzer(board);
    ProbeBattery(board);
    ProbeEnvironment(board);
    ProbeImu(board);
    ProbeRtc(board);
    ProbeDisplay(board);
    ProbeTouch(board);
    ProbeStorage(board);
    ProbeMicrophone(board);
    ESP_LOGI(TAG, "Board probe done");
}

void ShutdownMonitorTask(void*)
{
    auto& board = Board::GetInstance();

    Button* ok_button = nullptr;
    while (ok_button == nullptr) {
        ok_button = board.GetButtonOk();
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    while (ok_button->IsPressed()) {
        vTaskDelay(kShutdownPollTicks);
    }

    TickType_t pressed_since = 0;
    for (;;) {
        if (!ok_button->IsPressed()) {
            pressed_since = 0;
            vTaskDelay(kShutdownPollTicks);
            continue;
        }

        if (pressed_since == 0) {
            pressed_since = xTaskGetTickCount();
        }

        if ((xTaskGetTickCount() - pressed_since) >= kOkLongPressTicks) {
            ESP_LOGI(TAG, "OK long press detected, powering off");
            if (auto* buzzer = board.GetBuzzer(); buzzer != nullptr) {
                buzzer->Set(true);
                vTaskDelay(pdMS_TO_TICKS(80));
                buzzer->Set(false);
            }
            board.PrepareForDeepSleep();
            vTaskDelay(pdMS_TO_TICKS(100));
            board.PowerOff();

            for (;;) {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }

        vTaskDelay(kShutdownPollTicks);
    }
}

void StartShutdownMonitor()
{
    if (shutdown_task_handle != nullptr) {
        return;
    }

    const BaseType_t ret = xTaskCreate(ShutdownMonitorTask,
                                       "ok_shutdown",
                                       3072,
                                       nullptr,
                                       tskIDLE_PRIORITY + 2,
                                       &shutdown_task_handle);
    if (ret != pdPASS) {
        shutdown_task_handle = nullptr;
        ESP_LOGW(TAG, "failed to start shutdown monitor task");
    }
}

} // namespace


Hal& GetHAL()
{
    if (hal_instance == nullptr)
    {
        hal_instance = std::make_unique<Hal>();
    }
    return *hal_instance;
}

void Hal::init() {
    if (initialized_) {
        ESP_LOGI(TAG, "HAL already initialized");
        return;
    }

    auto& board = Board::GetInstance();
    ProbeBoardDevices(board);
    StartShutdownMonitor();
    initialized_ = true;
}

}  // namespace HAL
