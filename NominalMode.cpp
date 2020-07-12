/*
 * NominalMode.cpp
 *
 *  Created on: June 14th, 2020
 *      Author: Johan Monster
 */

// TODO:
// [ ] Test implementation
// [ ] Finalize documentation

#define NOMINALMODE_DEBUG

#include "NominalMode.h"


extern ADCSTelemetryContainer ADCSContainer;
extern EPSTelemetryContainer EPSContainer;
extern OBCVariableContainer variableContainer;


// Definition of NominalMode()
void NominalMode() {
    #ifdef NOMINALMODE_DEBUG
        Console::log("[DEBUG] NominalMode() activated.\n");
    #endif


    // === NoM-OBC-1 ===
    // Start by powering on ADCS to be able to receive ADCS telemetry data:
    if (ADCSContainer.getBusStatus() == 0) {

    }
    bool V2on = LineControl(1,1,0,0);

    #ifdef NOMINALMODE_DEBUG
        Console::log("[DEBUG] LineControl(1,1,0,0) called.\n");
    #endif


    // === NoM-OBC-2 ===
    unsigned short RateLimit = variableContainer.getRotateSpeedLimit();

    if (variableContainer.getRotateSpeed() >= RateLimit && variableContainer.getADCSState() == DISABLED) {
        variableContainer.setMode(SAFEMODE);
    }
    else if (variableContainer.getRotateSpeed() >= RateLimit && variableContainer.getADCSState() != DISABLED) {
        variableContainer.setMode(ADCSMODE);
    }



/*  // Alternative implementation
    // === NoM-OBC-2 ===
    // Since the ADCS is not implemented yet, no exact implementation can be defined here yet.
    // Therefore, a dummy implementation is used here, to be refined later.
    // It is assumed that the ADCS can output the rates in mRAD/s around the individual body axes.

    // Dummy values for current implementation:
    #ifdef NOMINALMODE_DEBUG
        signed long RateX = 80;
        signed long RateY = 4;
        signed long RateZ = 3;
    #endif

    // // TODO in future: replace the above with something like this:
    // signed long RateX = ADCSContainer.getRateX();
    // signed long RateY = ADCSContainer.getRateY();
    // signed long RateZ = ADCSContainer.getRateZ();


    // Define a total rate using the euclidean norm of angular rate vector.
    signed long TotalRate = (RateX^2 + RateY^2 + RateZ^2)^0.5;

    #ifdef NOMINALMODE_DEBUG
        Console::log("[DEBUG] Detected total rate of %d.\n",TotalRate);
    #endif

    // If the rates are in mRAD/s, a rate of 105 corresponds to just over 1 rotation per minute.
    signed long RateLimit = 105;

    // If total rate exceeds rate limit, enter ADCS mode.
    // Therefore ADCS Mode is entered when rotation around an individual axis is large enough, or
    // there is a significant rotation around two or more axes.
    if(TotalRate >= RateLimit) {
        variableContainer.setMode(ADCSMODE);
        #ifdef NOMINALMODE_DEBUG
            Console::log("[DEBUG] Total rate %d over limit. Switching to ADCS Mode.\n",TotalRate-RateLimit);
        #endif
    }
*/

    // === NoM-OBC-3 ===
    // - Not implemented.

    // === NoM-OBC-4 ===
    // - Not implemented.
}


