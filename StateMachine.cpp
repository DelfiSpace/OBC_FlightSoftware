/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#include "StateMachine.h"
#include "Communication.h"
#include "ActivationMode.h"
#include "Console.h"
#include "ResetService.h"
#include "ADBTelemetryContainer.h"
#include "ADCSTelemetryContainer.h"
#include "COMMSTelemetryContainer.h"
#include "EPSTelemetryContainer.h"
#include "PROPTelemetryContainer.h"

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

    // TDEM-OBC-6: Update time
    upTime++;
    totalUpTime++;

    // TDEM-OBC-1: Kick the external watchdog (time window: 2.5s)
    reset.refreshConfiguration();
    reset.kickExternalWatchDog();

    // TDEM-OBC-4: Request telemetry from active modules
    // Testing!
    Console::log("upTime: %d, Ping COMMS: %d", upTime, PingModule(COMMS));
    Console::log("upTime: %d, Ping EPS: %d", upTime, PingModule(EPS));

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
}
