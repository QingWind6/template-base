#pragma once

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

#define HAL_BOARD_NAME "new_board"

#define HAL_LED_STATUS GPIO_NUM_2
#define HAL_BUZZER GPIO_NUM_NC

#define HAL_BTN_UP GPIO_NUM_NC
#define HAL_BTN_DOWN GPIO_NUM_NC
#define HAL_BTN_OK GPIO_NUM_0

#define HAL_EPD_SCK GPIO_NUM_12
#define HAL_EPD_MOSI GPIO_NUM_11
#define HAL_EPD_CS GPIO_NUM_10
#define HAL_EPD_DC GPIO_NUM_9
#define HAL_EPD_RST GPIO_NUM_8
#define HAL_EPD_BUSY GPIO_NUM_7

#define HAL_BATTERY_ADC_UNIT ADC_UNIT_1
#define HAL_BATTERY_ADC_CHANNEL ADC_CHANNEL_0
#define HAL_BATTERY_CHARGE_DETECT GPIO_NUM_NC
