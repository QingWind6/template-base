#include "buzzer.h"

#include <esp_err.h>
#include <esp_log.h>

#include "Arduino.h"

namespace {
constexpr const char* kTag = "Buzzer";
}

bool Buzzer::Init(gpio_num_t gpio_num, uint8_t on_level)
{
    if(gpio_num == GPIO_NUM_NC || !GPIO_IS_VALID_OUTPUT_GPIO(gpio_num))
    {
        ESP_LOGE(kTag, "invalid buzzer gpio: %d", static_cast<int>(gpio_num));
        return false;
    }
    if(on_level > 1)
    {
        ESP_LOGE(kTag, "invalid buzzer on_level: %u", static_cast<unsigned>(on_level));
        return false;
    }

    gpio_config_t cfg = {};
    cfg.pin_bit_mask = (1ULL << gpio_num);
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_DISABLE;

    const esp_err_t err = gpio_config(&cfg);
    if(err != ESP_OK)
    {
        ESP_LOGE(kTag, "gpio_config failed, gpio=%d err=%s", static_cast<int>(gpio_num), esp_err_to_name(err));
        return false;
    }

    gpio_num_ = gpio_num;
    on_level_ = on_level;
    initialized_ = true;
    tone_started_ = false;

    Set(false);
    return true;
}

void Buzzer::Set(bool on)
{
    if(!initialized_)
    {
        ESP_LOGW(kTag, "Set called before Init");
        return;
    }

    if(tone_started_)
    {
        NoTone();
    }

    const int level = on ? on_level_ : (on_level_ ? 0 : 1);
    const esp_err_t err = gpio_set_level(gpio_num_, level);
    if(err != ESP_OK)
    {
        ESP_LOGE(kTag, "gpio_set_level failed, gpio=%d err=%s", static_cast<int>(gpio_num_), esp_err_to_name(err));
    }
}

bool Buzzer::Tone(uint32_t frequency_hz, uint32_t duration_ms)
{
    if(!initialized_)
    {
        ESP_LOGW(kTag, "Tone called before Init");
        return false;
    }

    if(frequency_hz == 0U)
    {
        ESP_LOGE(kTag, "Tone frequency must be > 0");
        return false;
    }

    tone(static_cast<uint8_t>(gpio_num_),
         static_cast<unsigned int>(frequency_hz),
         static_cast<unsigned long>(duration_ms));
    tone_started_ = true;
    return true;
}

void Buzzer::NoTone()
{
    if(!initialized_)
    {
        ESP_LOGW(kTag, "NoTone called before Init");
        return;
    }

    if(!tone_started_)
    {
        return;
    }

    noTone(static_cast<uint8_t>(gpio_num_));
    tone_started_ = false;
}
