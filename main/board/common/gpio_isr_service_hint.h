#pragma once

namespace Common {

inline bool& GpioIsrServiceInstalledHint()
{
    static bool installed = false;
    return installed;
}

inline void SetGpioIsrServiceInstalledHint(bool installed)
{
    GpioIsrServiceInstalledHint() = installed;
}

inline bool IsGpioIsrServiceInstalledHintSet()
{
    return GpioIsrServiceInstalledHint();
}

} // namespace Common
