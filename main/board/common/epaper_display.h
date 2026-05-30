#pragma once

#include <cstddef>
#include <cstdint>

#include "display.h"

enum class EpaperRefreshMode : uint8_t {
    Full = 0,
    Partial,
    Gray4,
};

using EpaperArea = DisplayArea;
using EpaperPixelFormat = DisplayPixelFormat;

class EpaperDisplay : public Display {
public:
    ~EpaperDisplay() override = default;
    DisplayType GetType() const override;

    virtual bool Prepare(EpaperRefreshMode mode) = 0;
    virtual bool WriteBitmap(const EpaperArea& area,
                             const void* pixels,
                             size_t src_stride_bytes,
                             EpaperPixelFormat input_format,
                             EpaperRefreshMode mode) = 0;
    //4_gray_surpport: write 2bpp gray4 data
    virtual bool WriteBitmapGray4(const EpaperArea& area,
                                   const uint8_t* pixels,
                                   size_t src_stride_bytes) { return false; }
    virtual bool WriteBitmapDiff(const EpaperArea& area,
                                 const void* previous_pixels,
                                 const void* current_pixels,
                                 size_t src_stride_bytes,
                                 EpaperPixelFormat input_format,
                                 EpaperRefreshMode mode) = 0;
    virtual bool Commit(const EpaperArea* area, EpaperRefreshMode mode) = 0;
    virtual bool Clear(bool white, EpaperRefreshMode mode) = 0;
    virtual bool Wakeup(EpaperRefreshMode mode) = 0;

    bool Clear(bool white = true) override;
    bool Wakeup() override;

    bool RefreshArea(const EpaperArea* area,
                     const void* pixels,
                     size_t src_stride_bytes,
                     EpaperPixelFormat input_format,
                     EpaperRefreshMode mode);
};
