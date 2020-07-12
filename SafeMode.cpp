/*
 * SafeMode.cpp
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// TODO:
// [ ] Test implementation
// [ ] Finalize documentation

#define SAFEMODE_DEBUG

#include "SafeMode.h"

extern EPSTelemetryContainer EPSContainer;
extern OBCVariableContainer variableContainer;


void SafeMode() {
    #ifdef SAFEMODE_DEBUG
        Console::log("[DEBUG] SafeMode() activated.\n");
    #endif
    // === SfM-OBC-1 ===
    // Dump telemetry in FRAM to SD card
    // - To be implemented later.

    // === SfM-OBC-2 ===
    // - Not implemented.

    // === SfM-OBC-3 ===
    // Command EPS to turn off other power lines except V1
    bool V1on = LineControl(1,0,0,0);

    #ifdef SAFEMODE_DEBUG
        Console::log("[DEBUG] LineControl(1,0,0,0) called.\n");
    #endif


    // === SfM-OBC-4 ===
    // If battery voltage > SM voltage and results of health

    // === Mode exit conditions ====
    // To avoid satellite getting stuck in Safe Mode when unexpected behaviour occurs,
    //   preferably implement additional conditions in future.

    // Exit condition 1: Battery voltage is over some threshold (here arbitrarily set to 3800 mV)
    if(EPSContainer.getBattVoltage() > 3800) {
        variableContainer.setMode(NOMINALMODE)
    }

}
