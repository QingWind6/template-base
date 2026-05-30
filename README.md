# template-base

A project template for ESP32 applications.

## Layout

- `components/`: project components.
- `main/app/`: application logic.
- `main/hal/`: shared hardware helpers.
- `main/board/common/interfaces/`: board capability interfaces.
- `main/board/common/devices/`: generic reusable device implementations.

Build with ESP-IDF:

```sh
idf.py set-target esp32s3
idf.py build
```
