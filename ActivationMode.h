/*
 *  ActivationMode.h
 *
 *  Created on: June 10, 2020
 *      Author: Zhuoheng Li
 */

#ifndef ACTIVATIONMODE_H_
#define ACTIVATIONMODE_H_
#include "Communication.h"
#include <OBCVariableContainer.h>
#include "Console.h"
#include "StateMachine.h"
void ActivationMode(Mode *currentMode, unsigned long totalUptime);
bool CommandEPS();

#endif /* ACTIVATIONMODE_H_ */
