/*
 * StateMachine.cpp
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#include "StateMachine.h"
#include "Communication.h"
#include "OBCFramAccess.h"
#include "ActivationMode.h"
#include "ADBTelemetryContainer.h"
#include "ADCSTelemetryContainer.h"
#include "COMMSTelemetryContainer.h"
#include "EPSTelemetryContainer.h"
#include "OBCTelemetryContainer.h"
#include "PROPTelemetryContainer.h"
#include "Console.h"
#include "ResetService.h"
#include "DeployMode.h"

#ifdef STATEMACHINE_DEBUG
    #include "DelfiPQcore.h"
#endif

Mode currentMode;
unsigned long upTime;
unsigned long totalUpTime;

extern ResetService reset;
extern HousekeepingService<OBCTelemetryContainer> hk;
extern MB85RS fram;
extern OBCVariableContainer variableContainer;
extern ADBTelemetryContainer ADBContainer;
extern ADCSTelemetryContainer ADCSContainer;
extern COMMSTelemetryContainer COMMSContainer;
extern EPSTelemetryContainer EPSContainer;
extern PROPTelemetryContainer PROPContainer;

void acquireTelemetry(OBCTelemetryContainer *tc)
{
    // set uptime in telemetry
    tc->setUpTime(upTime);

    float temp = ADCManager::getTempMeasurement();
    tc->setTemp(temp);

    uint16_t volt = ADCManager::getMeasurementVolt(ADC_MEM1);
    tc->setVoltage(volt);
}

void StateMachineInit()
{

#ifdef STATEMACHINE_DEBUG
    // Use it if you want to clear FRAM during debugging
    // fram.erase();
#endif

    // AcM-OBC-1: Load data from FRAM
    if (OBCFramRead(fram, OBCFRAM_VARIABLES_ADDR, variableContainer.getArray(), variableContainer.size()) == FRAM_OPERATION_SUCCESS)
    {
        variableContainer.NormalInit();

        currentMode = ACTIVATIONMODE;
        upTime = 0;
        totalUpTime = variableContainer.getTotalUpTime();
        variableContainer.setBootCount(variableContainer.getBootCount() + 1);
    }
    else
    {
        variableContainer.FirstBootInit(); // Including the BootCount

        currentMode = ACTIVATIONMODE;
        upTime = 0;
        totalUpTime = 0;
    }

    // TODO: AcM-OBC-2: Copy data from FRAM to the SD card

}

void StateMachine()
{
    /* put TDEM code here */
    char response;

    // Performance test
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

    //TDEM-OBC-4: acquire telemetry from OBC
    hk.acquireTelemetry(acquireTelemetry);

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

    // TDEM-OBC-9: Save variables in the variable container.
    variableContainer.setUpTime(upTime);
    variableContainer.setTotalUpTime(totalUpTime);

    // TDEM-OBC-9: Save containers in FRAM. TODO: error handling
    OBCFramWrite(fram, OBCFRAM_ADBTM_ADDR, ADBContainer.getArray(), ADBContainer.size());
    OBCFramWrite(fram, OBCFRAM_ADCSTM_ADDR, ADCSContainer.getArray(), ADCSContainer.size());
    OBCFramWrite(fram, OBCFRAM_COMMSTM_ADDR, COMMSContainer.getArray(), COMMSContainer.size());
    OBCFramWrite(fram, OBCFRAM_EPSTM_ADDR, EPSContainer.getArray(), EPSContainer.size());
    OBCFramWrite(fram, OBCFRAM_PROPTM_ADDR, PROPContainer.getArray(), PROPContainer.size());
    OBCFramWrite(fram, OBCFRAM_VARIABLES_ADDR, variableContainer.getArray(), variableContainer.size());

    //todo find where SM voltage is given
    //Check if voltage is high enough, else go into safe mode
    if(EPSContainer.getBattVoltage() < 3600 && currentMode != ACTIVATIONMODE && currentMode != DEPLOYMENTMODE) { //smaller then SM voltage parameter
        currentMode = SAFEMODE;
    }
    else {
        //send telemetry
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
