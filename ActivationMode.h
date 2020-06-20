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

//This function can be called from Statemachine
//and is therefore the only one in the hpp
void ActivationMode(OBCDataContainer *c, variables *var);



#endif /* ACTIVATIONMODE_HPP_ */
