set(BOARD_COMMON_SRCS
    "board/common/board.cc"
    "board/common/button.cc"
    "board/common/display.cc"
    "board/common/epaper_display.cc"
    "board/common/i2c_device.cc"
    "board/common/led.cc"
)

if(CONFIG_ARDUINO)
    list(APPEND BOARD_COMMON_SRCS
        "board/common/buzzer.cc"
    )
endif()

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
        ${BOARD_INCLUDE_DIRS}
    REQUIRES
        button
        esp_adc
        esp_driver_gpio
        esp_driver_i2c
)
