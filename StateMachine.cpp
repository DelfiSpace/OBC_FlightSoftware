/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#define STATEMACHINE_DEBUG

#include "ActivationMode.h"
#include "DeployMode.h"
#include "SafeMode.h"
#include "ADCSMode.h"
//#include "NominalMode.h"
#include "Communication.h"
#include "OBCFramAccess.h"
#include "ADBTelemetryContainer.h"
#include "ADCSTelemetryContainer.h"
#include "COMMSTelemetryContainer.h"
#include "EPSTelemetryContainer.h"
#include "PROPTelemetryContainer.h"
#include "ResetService.h"
#include "HouseKeepingService.h"

#ifdef STATEMACHINE_DEBUG
    #include "Console.h"
#endif

extern OBCTelemetryContainer OBCContainer;
extern ADBTelemetryContainer ADBContainer;
extern ADCSTelemetryContainer ADCSContainer;
extern COMMSTelemetryContainer COMMSContainer;
extern EPSTelemetryContainer EPSContainer;
extern PROPTelemetryContainer PROPContainer;
extern ResetService reset;
extern HousekeepingService<OBCTelemetryContainer> hk;
extern MB85RS fram;

extern void acquireTelemetry(OBCTelemetryContainer *tc);

void StateMachineInit()
{
#ifdef STATEMACHINE_DEBUG
    // Use it if you want to clear FRAM during debugging
    // fram.erase();
#endif

    // Load data from FRAM
    if (OBCFramRead(fram, OBCFRAM_VARIABLES_ADDR, OBCContainer.getArray(), OBCContainer.size()) == FRAM_OPERATION_SUCCESS)
    {
        OBCContainer.NormalInit();
    }
    else
    {
        OBCContainer.FirstBootInit(); // Including the BootCount
    }

    // TODO: Copy data from FRAM to the SD card

}

void StateMachine()
{
    // Kick the external watchdog (time window: 2.5s)
    reset.refreshConfiguration();
    reset.kickExternalWatchDog();

    // Acquire telemetry from OBC
    hk.acquireTelemetry(acquireTelemetry);

    // Request telemetry from active modules
    char response;

    response = RequestTelemetry(ADB, &ADBContainer);
    OBCContainer.setADBResponse(response);

    response = RequestTelemetry(ADCS, &ADCSContainer);
    OBCContainer.setADCSResponse(response);

    response = RequestTelemetry(COMMS, &COMMSContainer);
    OBCContainer.setCOMMSResponse(response);

    response = RequestTelemetry(EPS, &EPSContainer);
    OBCContainer.setEPSResponse(response);

    response = RequestTelemetry(PROP, &PROPContainer);
    OBCContainer.setPROPResponse(response);

    // Save containers in FRAM. TODO: error handling
    OBCFramWrite(fram, OBCFRAM_ADBTM_ADDR, ADBContainer.getArray(), ADBContainer.size());
    OBCFramWrite(fram, OBCFRAM_ADCSTM_ADDR, ADCSContainer.getArray(), ADCSContainer.size());
    OBCFramWrite(fram, OBCFRAM_COMMSTM_ADDR, COMMSContainer.getArray(), COMMSContainer.size());
    OBCFramWrite(fram, OBCFRAM_EPSTM_ADDR, EPSContainer.getArray(), EPSContainer.size());
    OBCFramWrite(fram, OBCFRAM_PROPTM_ADDR, PROPContainer.getArray(), PROPContainer.size());
    OBCFramWrite(fram, OBCFRAM_VARIABLES_ADDR, OBCContainer.getArray(), OBCContainer.size());

    // Check if voltage is high enough, else go into safe mode
    if(EPSContainer.getBattVoltage() < OBCContainer.getSMVoltage() && OBCContainer.getMode() != ACTIVATIONMODE && OBCContainer.getMode() != DEPLOYMENTMODE)
    {
        OBCContainer.setMode(SAFEMODE);
    }

    switch(OBCContainer.getMode())
    {
        case ACTIVATIONMODE:
            ActivationMode(&OBCContainer);
            break;
        case DEPLOYMENTMODE:
            DeployMode(&OBCContainer, ADBContainer);
            break;
        case SAFEMODE:
            SafeMode(&OBCContainer);
            break;
        case ADCSMODE:
            ADCSMode(&OBCContainer, ADCSContainer);
            break;
        case NOMINALMODE:
            //run nominal mode code
            break;
     }

}
