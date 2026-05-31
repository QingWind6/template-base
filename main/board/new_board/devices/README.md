# new_board devices

Place board-specific device implementations here. These classes adapt concrete
chips, buses, and GPIO wiring to the common interfaces in `board/common`.

Use this directory for code that is specific to `new_board`, for example:

- an e-paper panel driver implementing `EpaperDisplay`
- a fuel-gauge or ADC battery reader implementing `BatteryGauge`
- an IMU driver implementing `ImuDevice`
- a touch controller implementing `TouchDevice`
- a microphone or storage implementation for this board's wiring

Keep reusable, board-independent helpers in `board/common` instead.

## Integration

1. Add the driver `.cc` / `.h` files in this directory.
2. Use pin and bus definitions from `../config.h`.
3. Initialize the device from `../new_board.cpp`.
4. Store the device as a member of `NewBoard`.
5. Override the matching getter from `Board`, returning `nullptr` when the
   device is unavailable.
6. Add the driver source file to `BOARD_SRCS` in `main/CMakeLists.txt`.

The current `new_board` implementation already creates the shared I2C master bus,
the OK button, and the status LED. New I2C devices should reuse the board's I2C
bus instead of creating a second bus.
