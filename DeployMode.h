/*
 * DeployMode.h
 *
 *  Created on: Jun 14, 2020
 *      Author: tom-h
 */

#ifndef DEPLOYMODE_H_
#define DEPLOYMODE_H_

#include "OBCVariableContainer.h"
#include "OBC.h"
#include "Console.h"
#include "Communication.h"

void DeployMode(OBCVariableContainer *OBCVC, ADBTelemetryContainer *ADBTC, Mode *currentMode, unsigned long totalUptime);

#endif /* DEPLOYMODE_H_ */
