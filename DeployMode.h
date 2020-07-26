/*
 * DeployMode.h
 *
 *  Created on: Jun 14, 2020
 *      Author: tom-h
 */

#ifndef DEPLOYMODE_H_
#define DEPLOYMODE_H_

#include "ADBTelemetryContainer.h"
#include "OBCTelemetryContainer.h"

/**
 *
 *  Fixed logic of the deployment mode
 *
 *  Input:
 *      OBCContainer->getTotalUpTime()
 *      OBCContainer->getDeployState()
 *      OBCContainer->getBusVoltage()
 *      OBCContainer->getDeployVoltage()
 *      OBCContainer->getEndOfDeployState()
 *      OBCContainer->getDelayingDeployPeriod()
 *      OBCContainer->getForcedDeployPeriod()
 *      ADBContainer
 *  Output:
 *      OBCContainer->setMode()
 *      OBCContainer->setDeployState()
 *      OBCContainer->setEndOfDeployState()
 *
 */
void DeployMode(OBCTelemetryContainer *OBCContainer, ADBTelemetryContainer ADBContainer);

#endif /* DEPLOYMODE_H_ */
