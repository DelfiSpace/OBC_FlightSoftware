/*
 *  ActivationMode.h
 *
 *  Created on: June 10, 2020
 *      Author: Zhuoheng Li
 */

#ifndef ACTIVATIONMODE_H_
#define ACTIVATIONMODE_H_

#include "OBCTelemetryContainer.h"

/**
 *
 *  Fixed logic of the activation mode
 *
 *  Input:
 *      OBCContainer->getTotalUpTime()
 *      OBCContainer->getEndOfActivation()
 *  Output:
 *      OBCContainer->setMode()
 *
 */
void ActivationMode(OBCTelemetryContainer *OBCContainer);

#endif /* ACTIVATIONMODE_H_ */
