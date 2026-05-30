#include "epaper_display.h"

DisplayType EpaperDisplay::GetType() const
{
    return DisplayType::Epaper;
}

bool EpaperDisplay::Clear(bool white)
{
    return Clear(white, EpaperRefreshMode::Full);
}

bool EpaperDisplay::Wakeup()
{
    return Wakeup(EpaperRefreshMode::Full);
}

bool EpaperDisplay::RefreshArea(const EpaperArea* area,
                                const void* pixels,
                                size_t src_stride_bytes,
                                EpaperPixelFormat input_format,
                                EpaperRefreshMode mode)
{
    if(!Prepare(mode))
    {
        return false;
    }

    if(area == nullptr || pixels == nullptr)
    {
        return Commit(area, mode);
    }

    if(!WriteBitmap(*area, pixels, src_stride_bytes, input_format, mode))
    {
        return false;
    }

    return Commit(area, mode);
}
