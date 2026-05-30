#ifndef LED_H_
#define LED_H_

#include <driver/gpio.h>

class Led {
public:
    // 初始化：设置LED引脚 + LED亮时的电平(on_level: 0/1)
    bool Init(gpio_num_t gpio_num, uint8_t on_level = 1, bool open_drain = false);

    // 设置LED亮灭：on=true亮，on=false灭
    void Set(bool on);

private:
    gpio_num_t gpio_num_ = GPIO_NUM_NC;
    uint8_t on_level_ = 1;
    bool initialized_ = false;
};

#endif // LED_H_
