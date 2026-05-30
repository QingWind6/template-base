#pragma once

#include "esp_adc/adc_oneshot.h"

#define HAL_BOARD_NAME "new_board"

// ### -------------------------------------------------- ###
//                         INPUT
// ### -------------------------------------------------- ###

#define HAL_BTN_OK                  0

// ### -------------------------------------------------- ###
//                       INDICATORS
// ### -------------------------------------------------- ###

#define HAL_LED_STATUS              2

// ### -------------------------------------------------- ###
//                          I2C
// ### -------------------------------------------------- ###

#define HAL_I2C_SCL                 4
#define HAL_I2C_SDA                 5

// ### -------------------------------------------------- ###
//                        EPAPER
// ### -------------------------------------------------- ###

#define HAL_EPD_BUSY                7
#define HAL_EPD_RST                 8
#define HAL_EPD_DC                  9
#define HAL_EPD_CS                  10
#define HAL_EPD_MOSI                11
#define HAL_EPD_SCK                 12

// ### -------------------------------------------------- ###
//                        BATTERY
// ### -------------------------------------------------- ###

#define HAL_BATTERY_ADC_UNIT        ADC_UNIT_1
#define HAL_BATTERY_ADC_CHANNEL     ADC_CHANNEL_0
#define HAL_BATTERY_CHARGE_DETECT   6
