#pragma once

#include "power_managed.h"

class Storage : public PowerManaged {
public:
    ~Storage() override = default;

    virtual bool IsInserted() const = 0;
    virtual bool Mount(const char* base_path) = 0;
    virtual void Unmount() = 0;
    virtual bool IsMounted() const = 0;
    virtual const char* GetBasePath() const = 0;
};
