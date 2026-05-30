set(BOARD_COMMON_SRCS
    "board/common/board.cc"
    "board/common/devices/gpio_led.cc"
    "board/common/devices/gpio_buzzer.cc"
    "board/common/devices/adc_battery.cc"
    "board/common/devices/gpio_display.cc"
    "board/common/devices/gpio_button.cc"
)

set(BOARD_SRCS)
set(BOARD_INCLUDE_DIRS)

if(NOT DEFINED CONFIG_BOARD_NEW_BOARD OR CONFIG_BOARD_NEW_BOARD)
    list(APPEND BOARD_SRCS
        "board/new_board/new_board.cpp"
    )
    list(APPEND BOARD_INCLUDE_DIRS
        "board/new_board"
        "board/new_board/devices"
    )
else()
    message(FATAL_ERROR "No board selected")
endif()

idf_component_register(
    SRCS
        "main.cpp"
        "app/app.cc"
        "hal/hal.cpp"
        ${BOARD_COMMON_SRCS}
        ${BOARD_SRCS}
    INCLUDE_DIRS
        "."
        "app"
        "hal"
        "board/common"
        "board/common/interfaces"
        "board/common/devices"
        ${BOARD_INCLUDE_DIRS}
    REQUIRES
        esp_adc
        esp_driver_gpio
)
