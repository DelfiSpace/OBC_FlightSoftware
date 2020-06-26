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

extern ResetService reset;
extern MB85RS fram;
extern OBCVariableContainer variableContainer;
extern ADBTelemetryContainer ADBContainer;
extern ADCSTelemetryContainer ADCSContainer;
extern COMMSTelemetryContainer COMMSContainer;
extern EPSTelemetryContainer EPSContainer;
extern PROPTelemetryContainer PROPContainer;

void StateMachineInit()
{
    // AcM-OBC-1: Load data from FRAM


    // TODO: AcM-OBC-2: Copy data from FRAM to the SD card

}

void StateMachine()
{
    /* put TDEM code here */
    char response;

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
    response = RequestTelemetry(ADB, &ADBContainer);
    variableContainer.setADBResponse(response);

    response = RequestTelemetry(ADCS, &ADCSContainer);
    variableContainer.setADCSResponse(response);

    response = RequestTelemetry(COMMS, &COMMSContainer);
    variableContainer.setCOMMSResponse(response);

    response = RequestTelemetry(EPS, &EPSContainer);
    variableContainer.setEPSResponse(response);

    response = RequestTelemetry(PROP, &PROPContainer);
    variableContainer.setPROPResponse(response);

    // TDEM-OBC-9: Save data in dataContainer. Save dataContainer in FRAM
    if(fram->ping())
    {
        fram.write(ADBTELEMETRY_FRAM_OFFSET, ADBContainer.getArray(), ADBTELEMETRY_FRAM_SIZE);
        fram.write(ADCSTELEMETRY_FRAM_OFFSET, ADCSContainer.getArray(), ADCSTELEMETRY_FRAM_SIZE);
        fram.write(COMMSTELEMETRY_FRAM_OFFSET, COMMSContainer.getArray(), COMMSTELEMETRY_FRAM_SIZE);
        fram.write(EPSTELEMETRY_FRAM_OFFSET, EPSContainer.getArray(), EPSTELEMETRY_FRAM_SIZE);
        fram.write(PROPTELEMETRY_FRAM_OFFSET, PROPContainer.getArray(), PROPTELEMETRY_FRAM_SIZE);
        fram.write(OBCVARIABLE_FRAM_OFFSET, variableContainer.getArray(), OBCVARIABLE_FRAM_SIZE);
    }
    else
    {
#ifdef STATEMACHINE_DEBUG
        Console::log("StateMachine(): FRAM Unavailable!");
#endif
    }

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
