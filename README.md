# template-base

A project template for ESP32 applications.

## Layout

- `components/`: reusable ESP-IDF components.
- `main/app/`: application entry logic.
- `main/hal/`: shared hardware access layer.
- `main/board/common/`: board-facing abstractions and reusable device helpers.
- `main/board/new_board/`: the default board implementation.
- `main/board/new_board/devices/`: board-specific device drivers.

## Board Model

Applications should depend on `Board::GetInstance()` and the interfaces in
`main/board/common` instead of directly using a concrete board class.

`Board` exposes optional capabilities. A board returns `nullptr` for hardware it
does not provide.

- `Led`
- `Buzzer`
- `BatteryGauge`
- `Display` / `EpaperDisplay`
- `SdCard`
- `ImuDevice`
- `Microphone`
- `TouchDevice`
- `Button`

The default `new_board` template currently initializes:

- an I2C master bus on `HAL_I2C_SCL` / `HAL_I2C_SDA`
- one OK button on `HAL_BTN_OK`
- one status LED on `HAL_LED_STATUS`

Pin assignments and board identity live in `main/board/new_board/config.h`.

## Adding Hardware

1. Add reusable interfaces or generic helpers under `main/board/common`.
2. Add board-specific device implementations under
   `main/board/new_board/devices`.
3. Initialize the device in `main/board/new_board/new_board.cpp`.
4. Override the matching `Board` getter so application code can discover it.
5. Add any new source files to `main/CMakeLists.txt`.

Build with ESP-IDF:

```sh
idf.py set-target esp32s3
idf.py build
```
