#ifndef BUZZER_H_
#define BUZZER_H_

#include <cstdint>

#include <driver/gpio.h>

class Buzzer {
public:
    bool Init(gpio_num_t gpio_num, uint8_t on_level = 1);
    bool IsAvailable() const { return initialized_; }

    void Set(bool on);
    bool Tone(uint32_t frequency_hz, uint32_t duration_ms = 0);
    void NoTone();

private:
    gpio_num_t gpio_num_ = GPIO_NUM_NC;
    uint8_t on_level_ = 1;
    bool initialized_ = false;
    bool tone_started_ = false;
};

#endif // BUZZER_H_
