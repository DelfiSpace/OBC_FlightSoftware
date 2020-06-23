/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#define STATEMACHINE_DEBUG

#include "StateMachine.h"
#include "Communication.h"
#include "ActivationMode.h"
#include "ADBTelemetryContainer.h"
#include "ADCSTelemetryContainer.h"
#include "COMMSTelemetryContainer.h"
#include "EPSTelemetryContainer.h"
#include "PROPTelemetryContainer.h"
#include "Console.h"
#include "ResetService.h"

#ifdef STATEMACHINE_DEBUG
    #include "DelfiPQcore.h"
#endif

Mode currentMode;
unsigned long upTime;
unsigned long totalUpTime;
unsigned long OBCBootCount;

OBCDataContainer dataContainer;
ADBTelemetryContainer ADBContainer;
ADCSTelemetryContainer ADCSContainer;
COMMSTelemetryContainer COMMSContainer;
EPSTelemetryContainer EPSContainer;
PROPTelemetryContainer PROPContainer;

extern ResetService reset;

void StateMachineInit()
{
    // AcM-OBC-1: Load data from FRAM

    // AcM-OBC-2: Copy data from FRAM to the SD card

}

void StateMachine()
{
    /* put TDEM code here */

    // TODO: performance test
#ifdef STATEMACHINE_DEBUG
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    MAP_Timer32_setCount(TIMER32_0_BASE, UINT32_MAX);
    MAP_Timer32_startTimer(TIMER32_0_BASE, true);
#endif

    // TDEM-OBC-6: Update time
    upTime++;
    totalUpTime++;

    // TDEM-OBC-1: Kick the external watchdog (time window: 2.5s)
    reset.refreshConfiguration();
    reset.kickExternalWatchDog();

    // TDEM-OBC-4: Request telemetry from active modules
#ifdef STATEMACHINE_DEBUG
    int result;
    result = RequestTelemetry(COMMS, &COMMSContainer);
    Console::log("upTime: %d, Request telemetry from COMMS: %d", upTime, result);
    result = RequestTelemetry(EPS, &COMMSContainer);
    Console::log("upTime: %d, Request telemetry from EPS: %d", upTime, result);
#endif

    // TODO: TDEM-OBC-9: Save data in dataContainer. Save dataContainer in FRAM

    switch(currentMode)
    {
        case ACTIVATIONMODE:
            // ActivationMode(&dataContainer);
            break;
        case DEPLOYMENTMODE:
            //run safe mode code
            break;
        case SAFEMODE:
            //run deployment code
            break;
        case ADCSMODE:
            //run ADCS code
            break;
        case NOMINALMODE:
            //run nominal mode code
            break;
     }

#ifdef STATEMACHINE_DEBUG
    Console::log("StateMachine(): count of a loop: %d", UINT32_MAX - MAP_Timer32_getValue(TIMER32_0_BASE));
#endif
}
