/*
 * ActivationMode.cpp
 *
 *  Created on: May 20, 2020
 *      Author: tom-h
 */

//#include "PowerBusControl.h"
#include "OBCTelemetryContainer.h"

/**
 *
 *  Please refer to AvtivationMode.h
 *
 */
void ActivationMode(OBCTelemetryContainer *OBCContainer)
{

    //Command EPS to turn off all power lines except V1, return true if a fault occurs
    //TODO: Possibly do something if error occurs
//    PowerBusControl(1, 0, 0, 0);

    //check if current total uptime is longer than the specified time for deployment
    if (OBCContainer->getTotalUpTime() > OBCContainer->getEndOfActivation())
    {
        OBCContainer->setMode(DEPLOYMENTMODE);
        return;
    }
    else
        return;
}



