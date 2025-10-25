#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Arduino.h>
#include "Config.h"

class Context
{
private:
    Context()
    {
    }
    Context(const Context &) = delete;

    Context &operator=(const Context &) = delete;
    static Context instance;

public:
    // VARIABLES
    int loraRRSI = 0;

    float cpuTemperature = 0;
    float usedRam = 0;

    unsigned long uptimeInMinutes = 0;
    unsigned long sentPackets = 0;

    String deviceID;

    static Context &getInstance()
    {
        return instance;
    }
};

extern Context &ctx;
#endif // __CONTEXT_H__