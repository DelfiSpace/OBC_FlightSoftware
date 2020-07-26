/*
 * ADCSMode.cpp
 *
 *  Created on: July 26, 2020
 *      Author: Zhuoheng Li
 *
 */

#include "PowerBusControl.h"
#include "OBCTelemetryContainer.h"
#include "ADCSTelemetryContainer.h"

void ADCSMode(OBCTelemetryContainer *OBCContainer, ADCSTelemetryContainer ADCSContainer)
{
    // Check whether ADCS is disabled
    if (OBCContainer->getADCSState() == DISABLED)
    {
        OBCContainer->setMode(NOMINALMODE);
    }

    // Initialize the ADCS power line
    switch(OBCContainer->getADCSPowerState())
    {
    case UNINITIALIZED:
        //Command EPS to turn on power lines V1 and V2
        PowerBusControl(1, 1, 0, 0);
        OBCContainer->setADCSPowerState(INITIALIZED);
        break;
    case INITIALIZED:
        if (1) // TODO: if (ADCSHealthCheck(ADCSContainer))
            break;
        else
        {
            // TODO: 5 times and each time takes 3 second
            PowerBusControl(1, 0, 0, 0);
            PowerBusControl(1, 1, 0, 0);
            OBCContainer->setADCSPowerState(CYCLED);
            break;
        }
    case CYCLED:
        if (1) // TODO: if (ADCSHealthCheck(ADCSContainer))
        {
            OBCContainer->setADCSPowerState(INITIALIZED);
            break;
        }
        else
        {
            OBCContainer->setADCSPowerState(OFF);
            OBCContainer->setADCSState(DISABLED);
            OBCContainer->setMode(SAFEMODE);
            return;
        }
    }

    // Check the rotational speed and de-tumble
    switch(OBCContainer->getADCSPowerState())
    {
    case IDLE:
        if (5.5 > OBCContainer->getRotateSpeedLimit()) // TODO: get rotational speed from ADCS telemetry
        {
            OBCContainer->setADCSState(DETUMBLE);
            OBCContainer->setEndOfADCSState(OBCContainer->getTotalUpTime() + OBCContainer->getDetumblingPeriod());
        }
        else
            OBCContainer->setMode(NOMINALMODE);
        break;
   case DETUMBLE:
        if (5.5 < OBCContainer->getRotateSpeedLimit())
        {
            // TODO: send commands to ADCS module
            OBCContainer->setADCSState(IDLE);
            OBCContainer->setMode(NOMINALMODE);
        }
        else if (5.5 > OBCContainer->getRotateSpeedLimit() && OBCContainer->getTotalUpTime() > OBCContainer->getEndOfADCSState())
        {
            // TODO: send commands to ADCS module
            OBCContainer->setADCSState(DISABLED);
            OBCContainer->setMode(SAFEMODE);
        }
        else
        {
            // TODO: send commands to ADCS module
        }
        break;
    }

    return;
}
