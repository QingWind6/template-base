# template-base

A project template for ESP32 applications.

## Layout

- `components/`: project components.
- `main/app/`: application logic.
- `main/hal/`: shared hardware helpers.
- `main/board/common/interfaces/`: board capability interfaces.
- `main/board/common/devices/`: generic reusable device implementations.
- `main/board/new_board/`: concrete `new_board` implementation.
- `main/board/new_board/devices/`: board-specific chip implementations.

## Flash Layout

The partition table targets 8 MB flash:

- `ota_0`: 3 MB
- `ota_1`: 3 MB
- `littlefs`: remaining `0x1F0000`

Build with ESP-IDF:

```sh
idf.py set-target esp32s3
idf.py build
```

Build the `juggler` board variant with an isolated sdkconfig/build directory:

```sh
idf.py -B build_juggler -D SDKCONFIG=sdkconfig.juggler -D SDKCONFIG_DEFAULTS="sdkconfig.defaults;sdkconfig.defaults.juggler" build
```
