#pragma once

class LowPowerControllable {
public:
    virtual ~LowPowerControllable() = default;

    virtual bool EnterLowPower() { return true; }
    virtual bool ExitLowPower() { return true; }
};
