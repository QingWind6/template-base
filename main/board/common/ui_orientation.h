#pragma once

#include <cstdint>

namespace Ui {

enum class Rotation : uint8_t {
    k0 = 0,
    k90 = 1,
    k180 = 2,
    k270 = 3,
};

enum class MemoryLocation : uint8_t {
    kInternal = 0,
    kExternal = 1,
};

struct DisplayConfig {
    Rotation rotation = Rotation::k0;
    bool panel_mirror_x = false;
};

struct TouchConfig {
    bool swap_xy = false;
    bool mirror_x = false;
    bool mirror_y = false;
};

struct Config {
    DisplayConfig display = {};
    TouchConfig touch = {};
    bool reduce_motion = false;
    MemoryLocation lvgl_memory = MemoryLocation::kInternal;
    MemoryLocation lvgl_task_stack_memory = MemoryLocation::kInternal;
};

} // namespace Ui
