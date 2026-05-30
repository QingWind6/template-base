#pragma once

#include "power_managed.h"

#include <ctime>

class Rtc : public PowerManaged {
public:
    ~Rtc() override = default;

    virtual bool GetTime(std::tm* time) = 0;
    virtual bool SetTime(const std::tm& time) = 0;
};
