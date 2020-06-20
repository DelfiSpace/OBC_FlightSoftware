/*
 * StateMachine.h
 *
 *  Created on: May 19, 2020
 *      Author: tom-h
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "OBCDataContainer.h"

Mode currentMode;
unsigned long upTime;
unsigned long totalUpTime;
unsigned long OBCBootCount;

typedef struct variables {Mode currentMode; unsigned long upTime; unsigned long totalUpTime; unsigned long OBCBootCount;} variables;
void StateMachineInit();
void StateMachine();

#endif /* STATEMACHINE_H_ */
