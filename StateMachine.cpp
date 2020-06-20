/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#include "StateMachine.h"
#include "ActivationMode.h"
#include "Console.h"
#include "ResetService.h"
//#include "ADBTelemetryContainer.h"
//#include "ADCSTelemetryContainer.h"
//#include "COMMSTelemetryContainer.h"
//#include "EPSTelemetryContainer.h"
//#include "PROPTelemetryContainer.h"



variables * var;
OBCDataContainer * dataContainer;
//ADBTelemetryContainer ADBContainer;
//ADCSTelemetryContainer ADCSContainer;
//COMMSTelemetryContainer COMMSContainer;
//EPSTelemetryContainer EPSContainer;
//PROPTelemetryContainer PROPContainer;

//extern ResetService reset(GPIO_PORT_P4, GPIO_PIN0);

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
//    reset.refreshConfiguration();
//    reset.kickExternalWatchDog();

    // TDEM-OBC-4: Request telemetry from active modules
//    RequestTelemetry(currentMode, &ADBContainer, &ADCSContainer, &COMMSContainer,
//                     &EPSContainer, &PROPContainer);
//    ADBHealthCheck(ADBContainer);
//    ADCSHealthCheck(ADCSContainer);
//    COMMSHealthCheck(COMMSContainer);
//    EPSHealthCheck(EPSContainer);
//    PROPHealthCheck(PROPContainer);

    // TODO: TDEM-OBC-9: Save data in dataContainer. Save dataContainer in FRAM

    switch(currentMode)
    {
        case ACTIVATION:
            ActivationMode(dataContainer, var);
            break;
        case DEPLOYMENT:
            //run safe mode code
            break;
        case SAFE:
            //run deployment code
            break;
        case ADCS:
            //run ADCS code
            break;
        case NOMINAL:
            //run nominal mode code
            break;
     }
}
