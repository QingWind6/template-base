#pragma once

enum class LowPowerCapability {
    kNone,
    kImplicit,
    kExplicit,
};

class PowerManaged {
public:
    virtual ~PowerManaged() = default;

    virtual LowPowerCapability GetLowPowerCapability() const {
        return LowPowerCapability::kNone;
    }

    virtual bool EnterLowPowerMode() {
        return true;
    }

    virtual bool ExitLowPowerMode() {
        return true;
    }

    virtual bool IsInLowPowerMode() const {
        return false;
    }
};
