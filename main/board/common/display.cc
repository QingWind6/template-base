#include "display.h"

DisplayType Display::GetType() const
{
    return DisplayType::Unknown;
}

bool Display::Probe()
{
    return true;
}

bool Display::ClearWhite()
{
    return Clear(true);
}

bool Display::ClearBlack()
{
    return Clear(false);
}
