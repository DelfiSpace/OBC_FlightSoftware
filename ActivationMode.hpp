/*
 * ActivationMode.hpp
 *
 *  Created on: May 20, 2020
 *      Author: tom-h
 */
#ifndef ACTIVATIONMODE_HPP_
#define ACTIVATIONMODE_HPP_

#include "StateMachine.h"
#include "OBCTelemetryContainer.h"
#include "MB85RS.h"

namespace activation{

int BootCount;
//True when the 30 mins have past
bool TimerDone;
//This function can be called from Statemachine
//and is therefore the only one in the hpp
void Activation();

}

#endif /* ACTIVATIONMODE_HPP_ */
