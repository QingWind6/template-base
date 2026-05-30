#pragma once

class SdCard {
public:
    virtual ~SdCard() = default;

    virtual void init() {}
    virtual bool isAvailable() { return false; }
    virtual bool isInserted() { return false; }
    virtual bool mount(const char* base_path = "/sdcard")
    {
        (void)base_path;
        return false;
    }
    virtual void unmount() {}
    virtual bool isMounted() { return false; }
    virtual const char* getBasePath() { return nullptr; }
};
