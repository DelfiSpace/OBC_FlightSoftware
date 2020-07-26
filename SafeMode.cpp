/*
 * SafeMode.cpp
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

#define SAFEMODE_DEBUG

#include "PowerBusControl.h" // Makes line control easier.
#include "OBCTelemetryContainer.h"
#ifdef SAFEMODE_DEBUG
    #include "Console.h"
#endif

/**
 *
 *  Please refer to DeployMode.h
 *
 */
void SafeMode(OBCTelemetryContainer *OBCContainer)
{
    #ifdef SAFEMODE_DEBUG
        Console::log("SafeMode() activated.\n");
    #endif
    // === SfM-OBC-1 ===
    // Dump telemetry in FRAM to SD card
    // - To be implemented later.

    // === SfM-OBC-2 ===
    // Command EPS to turn off other power lines except V1
    PowerBusControl(1,0,0,0);

    #ifdef SAFEMODE_DEBUG
        Console::log("LineControl(1,0,0,0) called.\n");
    #endif

    // === SfM-OBC-3 ===
    // === Mode exit conditions ====
    // To avoid satellite getting stuck in Safe Mode when unexpected behaviour occurs,
    //   preferably implement additional conditions in future.

    // Exit condition 1: Battery voltage is over some threshold
    if(OBCContainer->getBusVoltage() > OBCContainer->getSMVoltage()) {
        OBCContainer->setMode(ADCSMODE);
    }

}
